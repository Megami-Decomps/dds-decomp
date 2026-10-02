"""Shared validation helpers for importing self-contained glTF 2.0 binaries."""

from __future__ import annotations

import json
import struct

import fld_model


class GltfImportError(ValueError):
    """Raised when a GLB cannot be mapped safely to native DDS data."""


GLB_HEADER = struct.Struct("<4sII")
GLB_CHUNK = struct.Struct("<II")
COMPONENT_FORMATS = {
    fld_model.UNSIGNED_BYTE: "B",
    fld_model.UNSIGNED_SHORT: "H",
    fld_model.UNSIGNED_INT: "I",
    fld_model.FLOAT: "f",
}
TYPE_WIDTHS = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4}


def decode_glb(data: bytes) -> tuple[dict, bytes]:
    """Read one self-contained glTF 2.0 binary document."""

    if len(data) < GLB_HEADER.size + GLB_CHUNK.size:
        raise GltfImportError("GLB is truncated")
    magic, version, total_size = GLB_HEADER.unpack_from(data)
    if magic != b"glTF" or version != 2:
        raise GltfImportError("input is not a glTF 2.0 binary")
    if total_size != len(data):
        raise GltfImportError(
            f"GLB header size is {total_size}, but file size is {len(data)}"
        )
    chunks = []
    offset = GLB_HEADER.size
    while offset < len(data):
        if offset + GLB_CHUNK.size > len(data):
            raise GltfImportError("GLB has a truncated chunk header")
        size, kind = GLB_CHUNK.unpack_from(data, offset)
        offset += GLB_CHUNK.size
        end = offset + size
        if end > len(data):
            raise GltfImportError("GLB has a truncated chunk")
        chunks.append((kind, data[offset:end]))
        offset = end
    if len(chunks) != 2 or chunks[0][0] != fld_model.GLB_JSON_CHUNK:
        raise GltfImportError(
            "GLB must contain one JSON chunk followed by one BIN chunk"
        )
    if chunks[1][0] != fld_model.GLB_BIN_CHUNK:
        raise GltfImportError("GLB second chunk is not binary data")
    try:
        document = json.loads(chunks[0][1].decode("utf-8"))
    except (UnicodeDecodeError, json.JSONDecodeError) as exc:
        raise GltfImportError(f"invalid GLB JSON: {exc}") from exc
    if not isinstance(document, dict):
        raise GltfImportError("GLB JSON root is not an object")
    buffers = document.get("buffers")
    if not isinstance(buffers, list) or len(buffers) != 1:
        raise GltfImportError("GLB must have exactly one embedded buffer")
    byte_length = buffers[0].get("byteLength") if isinstance(buffers[0], dict) else None
    binary = chunks[1][1]
    if (
        not isinstance(byte_length, int)
        or isinstance(byte_length, bool)
        or not 0 <= byte_length <= len(binary)
    ):
        raise GltfImportError("GLB buffer has an invalid byte length")
    if len(binary) - byte_length > 3 or any(binary[byte_length:]):
        raise GltfImportError("GLB binary padding is invalid")
    return document, binary[:byte_length]


def records(
    document: dict,
    binary: bytes,
    accessor_index: object,
    value_type: str,
    component_types: set[int],
    context: str,
    *,
    normalized: bool | None = None,
) -> tuple[tuple[int | float, ...], ...]:
    """Read a dense accessor after validating its type and bounds."""

    accessors = document.get("accessors")
    views = document.get("bufferViews")
    if (
        not isinstance(accessor_index, int)
        or isinstance(accessor_index, bool)
        or not isinstance(accessors, list)
    ):
        raise GltfImportError(f"{context} has no valid accessor")
    if not 0 <= accessor_index < len(accessors) or not isinstance(
        accessors[accessor_index], dict
    ):
        raise GltfImportError(f"{context} accessor is outside the GLB")
    accessor = accessors[accessor_index]
    if "sparse" in accessor:
        raise GltfImportError(f"{context} uses a sparse accessor")
    component_type = accessor.get("componentType")
    count = accessor.get("count")
    if (
        accessor.get("type") != value_type
        or component_type not in component_types
        or not isinstance(count, int)
        or isinstance(count, bool)
        or count < 0
    ):
        raise GltfImportError(f"{context} accessor has an incompatible format")
    if normalized is not None and accessor.get("normalized", False) is not normalized:
        raise GltfImportError(f"{context} accessor has incompatible normalization")
    view_index = accessor.get("bufferView")
    if (
        not isinstance(view_index, int)
        or isinstance(view_index, bool)
        or not isinstance(views, list)
        or not 0 <= view_index < len(views)
        or not isinstance(views[view_index], dict)
    ):
        raise GltfImportError(f"{context} has no valid buffer view")
    view = views[view_index]
    if view.get("buffer", 0) != 0:
        raise GltfImportError(f"{context} is not in the embedded buffer")
    code = COMPONENT_FORMATS[component_type]
    width = TYPE_WIDTHS[value_type]
    record_size = struct.calcsize("<" + code * width)
    stride = view.get("byteStride", record_size)
    if not isinstance(stride, int) or isinstance(stride, bool) or stride < record_size:
        raise GltfImportError(f"{context} has an invalid byte stride")
    view_start = view.get("byteOffset", 0)
    accessor_start = accessor.get("byteOffset", 0)
    view_size = view.get("byteLength")
    if (
        not isinstance(view_start, int)
        or isinstance(view_start, bool)
        or not isinstance(accessor_start, int)
        or isinstance(accessor_start, bool)
        or not isinstance(view_size, int)
        or isinstance(view_size, bool)
        or view_start < 0
        or accessor_start < 0
        or view_size < 0
    ):
        raise GltfImportError(f"{context} has invalid byte offsets")
    start = view_start + accessor_start
    end = start if not count else start + (count - 1) * stride + record_size
    if start < view_start or end > view_start + view_size or end > len(binary):
        raise GltfImportError(f"{context} accessor exceeds its buffer view")
    return tuple(
        struct.unpack_from("<" + code * width, binary, start + index * stride)
        for index in range(count)
    )


def f32(value: float) -> float:
    """Round one finite Python number through the native float32 format."""

    try:
        return struct.unpack("<f", struct.pack("<f", value))[0]
    except (OverflowError, struct.error) as exc:
        raise GltfImportError(
            f"value {value!r} cannot be represented as float32"
        ) from exc
