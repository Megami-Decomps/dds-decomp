#!/usr/bin/env python3
"""Export semantic FLD1 field models as a self-contained glTF 2.0 GLB file."""

from __future__ import annotations

import argparse
import json
import math
import struct
from dataclasses import dataclass
from pathlib import Path

import fld
import lb
import tmx


GLB_JSON_CHUNK = 0x4E4F534A
GLB_BIN_CHUNK = 0x004E4942
ARRAY_BUFFER = 34962
ELEMENT_ARRAY_BUFFER = 34963
FLOAT = 5126
UNSIGNED_BYTE = 5121
UNSIGNED_SHORT = 5123
UNSIGNED_INT = 5125


@dataclass
class GltfBuilder:
    document: dict
    binary: bytearray

    @classmethod
    def create(cls) -> "GltfBuilder":
        return cls(
            {
                "asset": {
                    "version": "2.0",
                    "generator": "dds-decomp FLD1 model exporter",
                },
                "scene": 0,
                "scenes": [{"nodes": []}],
                "nodes": [],
                "meshes": [],
                "materials": [
                    {
                        "name": "DDS vertex colors",
                        "doubleSided": True,
                        "pbrMetallicRoughness": {
                            "metallicFactor": 0.0,
                            "roughnessFactor": 1.0,
                        },
                    }
                ],
                "accessors": [],
                "bufferViews": [],
            },
            bytearray(),
        )

    def accessor(
        self,
        payload: bytes,
        component_type: int,
        value_type: str,
        count: int,
        *,
        target: int | None = None,
        minimum: list[float | int] | None = None,
        maximum: list[float | int] | None = None,
        normalized: bool = False,
    ) -> int:
        self.binary.extend(bytes((-len(self.binary)) & 3))
        offset = len(self.binary)
        self.binary.extend(payload)
        view = {"buffer": 0, "byteOffset": offset, "byteLength": len(payload)}
        if target is not None:
            view["target"] = target
        view_index = len(self.document["bufferViews"])
        self.document["bufferViews"].append(view)
        accessor = {
            "bufferView": view_index,
            "componentType": component_type,
            "count": count,
            "type": value_type,
        }
        if minimum is not None:
            accessor["min"] = minimum
        if maximum is not None:
            accessor["max"] = maximum
        if normalized:
            accessor["normalized"] = True
        index = len(self.document["accessors"])
        self.document["accessors"].append(accessor)
        return index

    def texture(self, texture: tmx.Texture) -> int:
        self.binary.extend(bytes((-len(self.binary)) & 3))
        offset = len(self.binary)
        png = tmx.encode_png(texture)
        self.binary.extend(png)
        view_index = len(self.document["bufferViews"])
        self.document["bufferViews"].append(
            {"buffer": 0, "byteOffset": offset, "byteLength": len(png)}
        )
        image_index = len(self.document.setdefault("images", []))
        self.document["images"].append(
            {
                "name": f"texture_{texture.index:03d}",
                "bufferView": view_index,
                "mimeType": "image/png",
                "extras": {
                    "ddsPixelStorageMode": tmx.PSM_NAMES[texture.psm],
                    "ddsMipmapCount": texture.mipmap_count,
                    "ddsClutStorageMode": texture.clut_psm,
                    "ddsTextureFlags": texture.texture_flags,
                },
            }
        )
        texture_index = len(self.document.setdefault("textures", []))
        self.document["textures"].append(
            {"name": f"texture_{texture.index:03d}", "source": image_index}
        )
        return texture_index


def _pack_floats(records: tuple[tuple[float, ...], ...]) -> bytes:
    return struct.pack(
        "<" + "f" * sum(len(record) for record in records),
        *(value for record in records for value in record),
    )


def add_marker_mesh(
    builder: GltfBuilder,
    name: str,
    material: int,
    size: float,
) -> int:
    """Add a reusable octahedral diagnostic marker."""

    positions = (
        (size, 0.0, 0.0),
        (-size, 0.0, 0.0),
        (0.0, size, 0.0),
        (0.0, -size, 0.0),
        (0.0, 0.0, size),
        (0.0, 0.0, -size),
    )
    triangles = (
        0, 2, 4, 2, 1, 4, 1, 3, 4, 3, 0, 4,
        2, 0, 5, 1, 2, 5, 3, 1, 5, 0, 3, 5,
    )
    position_accessor = builder.accessor(
        _pack_floats(positions),
        FLOAT,
        "VEC3",
        len(positions),
        target=ARRAY_BUFFER,
        minimum=[-size, -size, -size],
        maximum=[size, size, size],
    )
    index_accessor = builder.accessor(
        bytes(triangles),
        UNSIGNED_BYTE,
        "SCALAR",
        len(triangles),
        target=ELEMENT_ARRAY_BUFFER,
        minimum=[0],
        maximum=[5],
    )
    mesh_index = len(builder.document["meshes"])
    builder.document["meshes"].append(
        {
            "name": name,
            "primitives": [
                {
                    "attributes": {"POSITION": position_accessor},
                    "indices": index_accessor,
                    "material": material,
                    "mode": 4,
                }
            ],
        }
    )
    return mesh_index


def _normalized_quaternion(values: tuple[float, float, float, float]) -> list[float]:
    length = math.sqrt(sum(value * value for value in values))
    if length == 0.0:
        return [0.0, 0.0, 0.0, 1.0]
    return [value / length for value in values]


def _float_bits(values: tuple[float, ...]) -> list[str]:
    return [
        f"0x{struct.unpack('<I', struct.pack('<f', value))[0]:08x}"
        for value in values
    ]


def _set_transform_component(
    node: dict,
    key: str,
    values: tuple[float, ...],
    converted: list[float],
) -> None:
    if all(math.isfinite(value) for value in values):
        node[key] = converted
    else:
        node["extras"][f"ddsOmitted{key.title()}Bits"] = _float_bits(values)


def _euler_quaternion(x: float, y: float, z: float) -> list[float]:
    """Reproduce the quaternion written by the DDS Euler conversion routine."""

    sx, cx = math.sin(-x * 0.5), math.cos(-x * 0.5)
    sy, cy = math.sin(-y * 0.5), math.cos(-y * 0.5)
    sz, cz = math.sin(-z * 0.5), math.cos(-z * 0.5)
    return _normalized_quaternion(
        (
            sz * sy * cx + cz * cy * sx,
            cz * sy * cx - sz * cy * sx,
            sz * cy * cx + cz * sy * sx,
            cz * cy * cx - sz * sy * sx,
        )
    )


def _mesh_geometry(
    builder: GltfBuilder,
    mesh: fld.ModelMesh,
    meters_per_unit: float,
    context: str,
) -> tuple[dict[str, int], int, int]:
    arrays = (
        ("positions", mesh.positions),
        ("normals", mesh.normals),
        ("texture coordinates", mesh.texcoords),
        ("vertex attributes", mesh.attributes),
    )
    for label, records in arrays:
        if records is not None and any(
            not math.isfinite(value) for record in records for value in record
        ):
            raise fld.FldError(f"{context} has non-finite {label}")
    positions = tuple(
        tuple(value * meters_per_unit for value in position)
        for position in mesh.positions
    )
    attributes = {
        "POSITION": builder.accessor(
            _pack_floats(positions),
            FLOAT,
            "VEC3",
            len(positions),
            target=ARRAY_BUFFER,
            minimum=[min(record[axis] for record in positions) for axis in range(3)],
            maximum=[max(record[axis] for record in positions) for axis in range(3)],
        )
    }
    if mesh.normals is not None:
        attributes["NORMAL"] = builder.accessor(
            _pack_floats(mesh.normals),
            FLOAT,
            "VEC3",
            len(mesh.normals),
            target=ARRAY_BUFFER,
        )
    if mesh.texcoords is not None:
        attributes["TEXCOORD_0"] = builder.accessor(
            _pack_floats(mesh.texcoords),
            FLOAT,
            "VEC2",
            len(mesh.texcoords),
            target=ARRAY_BUFFER,
        )
    if mesh.attributes is not None:
        attributes["_DDS_ATTRIBUTE"] = builder.accessor(
            _pack_floats(mesh.attributes),
            FLOAT,
            "VEC4",
            len(mesh.attributes),
            target=ARRAY_BUFFER,
        )
    if mesh.colors is not None:
        attributes["COLOR_0"] = builder.accessor(
            bytes(min(value * 2, 0xFF) for color in mesh.colors for value in color),
            UNSIGNED_BYTE,
            "VEC4",
            len(mesh.colors),
            target=ARRAY_BUFFER,
            normalized=True,
        )

    indices = tuple(value for triangle in mesh.triangles for value in triangle[:3])
    index_accessor = builder.accessor(
        bytes(indices),
        UNSIGNED_BYTE,
        "SCALAR",
        len(indices),
        target=ELEMENT_ARRAY_BUFFER,
        minimum=[min(indices)],
        maximum=[max(indices)],
    )
    controls = bytes(triangle[3] for triangle in mesh.triangles)
    control_accessor = builder.accessor(
        controls,
        UNSIGNED_BYTE,
        "SCALAR",
        len(controls),
    )
    return attributes, index_accessor, control_accessor


def add_marker_mesh(
    builder: GltfBuilder,
    name: str,
    material: int,
    size: float,
) -> int:
    """Add a reusable octahedral diagnostic marker."""

    positions = (
        (size, 0.0, 0.0),
        (-size, 0.0, 0.0),
        (0.0, size, 0.0),
        (0.0, -size, 0.0),
        (0.0, 0.0, size),
        (0.0, 0.0, -size),
    )
    triangles = (
        0, 2, 4, 2, 1, 4, 1, 3, 4, 3, 0, 4,
        2, 0, 5, 1, 2, 5, 3, 1, 5, 0, 3, 5,
    )
    position_accessor = builder.accessor(
        _pack_floats(positions),
        FLOAT,
        "VEC3",
        len(positions),
        target=ARRAY_BUFFER,
        minimum=[-size, -size, -size],
        maximum=[size, size, size],
    )
    index_accessor = builder.accessor(
        bytes(triangles),
        UNSIGNED_BYTE,
        "SCALAR",
        len(triangles),
        target=ELEMENT_ARRAY_BUFFER,
        minimum=[0],
        maximum=[5],
    )
    mesh_index = len(builder.document["meshes"])
    builder.document["meshes"].append(
        {
            "name": name,
            "primitives": [
                {
                    "attributes": {"POSITION": position_accessor},
                    "indices": index_accessor,
                    "material": material,
                    "mode": 4,
                }
            ],
        }
    )
    return mesh_index


def _track_values(data: bytes, track: fld.ModelMotionTrack) -> tuple[tuple, ...]:
    _, _, width, value_kind = fld.MODEL_MOTION_VALUE_DIRECTIVES[track.format_name]
    if value_kind == "float":
        code = "f"
    elif value_kind == "s16":
        code = "h"
    elif value_kind == "u8":
        code = "B"
    else:
        raise AssertionError(value_kind)
    return tuple(
        struct.unpack_from(
            "<" + code * width,
            data,
            track.values + index * track.stride,
        )
        for index in range(track.count)
    )


def _clip_summary(clip: fld.ModelMotionClip | None) -> dict | None:
    if clip is None:
        return None
    summary = {"durationFrames": clip.duration, "reserved": clip.reserved}
    unordered = [
        index
        for index, track in enumerate(clip.tracks)
        if any(left >= right for left, right in zip(track.frames, track.frames[1:]))
    ]
    if unordered:
        summary["unorderedBindings"] = unordered
    return summary


def _add_animations(
    builder: GltfBuilder,
    data: bytes,
    resource_name: str,
    motion: fld.ModelMotionPlaybook,
    node_indices: tuple[int, ...],
    meters_per_unit: float,
    frames_per_second: float,
) -> None:
    for clip_index, clip in enumerate(motion.clips):
        if clip is None:
            continue
        animation = {
            "name": f"{resource_name}/clip_{clip_index}",
            "samplers": [],
            "channels": [],
            "extras": {
                "ddsDurationFrames": clip.duration,
                "ddsReserved": clip.reserved,
                "ddsIgnoredAssetBindings": sum(
                    binding.family == 1 for binding in motion.bindings
                ),
            },
        }
        skipped = []
        for binding_index, (binding, track) in enumerate(
            zip(motion.bindings, clip.tracks, strict=True)
        ):
            if binding.family != 0 or binding.selector == 4:
                continue
            if any(left >= right for left, right in zip(track.frames, track.frames[1:])):
                skipped.append(binding_index)
                continue
            values = _track_values(data, track)
            if any(
                isinstance(value, float) and not math.isfinite(value)
                for record in values
                for value in record
            ):
                raise fld.FldError(
                    f"model {resource_name} clip {clip_index} binding "
                    f"{binding_index} has non-finite motion values"
                )
            path: str
            converted: tuple[tuple[float, ...], ...]
            if binding.selector == 0:
                path = "translation"
                converted = tuple(
                    tuple(value * meters_per_unit for value in record)
                    for record in values
                )
            elif binding.selector == 1:
                path = "rotation"
                converted = tuple(tuple(_euler_quaternion(*record)) for record in values)
            elif binding.selector == 2:
                path = "scale"
                converted = tuple(tuple(float(value) for value in record) for record in values)
            elif binding.selector == 3:
                path = "rotation"
                converted = tuple(
                    tuple(
                        _normalized_quaternion(
                            tuple(float(value) / 4096.0 for value in record)
                        )
                    )
                    for record in values
                )
            else:
                continue
            times = tuple(float(frame) / frames_per_second for frame in track.frames)
            time_accessor = builder.accessor(
                struct.pack("<" + "f" * len(times), *times),
                FLOAT,
                "SCALAR",
                len(times),
                minimum=[min(times)],
                maximum=[max(times)],
            )
            output_accessor = builder.accessor(
                _pack_floats(converted),
                FLOAT,
                "VEC4" if path == "rotation" else "VEC3",
                len(converted),
            )
            sampler = len(animation["samplers"])
            animation["samplers"].append(
                {"input": time_accessor, "output": output_accessor, "interpolation": "LINEAR"}
            )
            animation["channels"].append(
                {
                    "sampler": sampler,
                    "target": {"node": node_indices[binding.target], "path": path},
                }
            )
        if skipped:
            animation["extras"]["ddsSkippedUnorderedBindings"] = skipped
        if animation["channels"]:
            builder.document.setdefault("animations", []).append(animation)


def _gltf_texture(
    builder: GltfBuilder,
    textures: tuple[tmx.Texture, ...],
    texture_cache: dict[int, int],
    index: int,
    context: str,
) -> tuple[int, bool]:
    if index < 0 or index >= len(textures):
        raise fld.FldError(
            f"{context} references texture {index}, but the bundle has "
            f"{len(textures)} textures"
        )
    texture = textures[index]
    if texture.index != index:
        raise fld.FldError(
            f"texture bundle entry {index} has unexpected index {texture.index}"
        )
    gltf_index = texture_cache.get(index)
    if gltf_index is None:
        gltf_index = builder.texture(texture)
        texture_cache[index] = gltf_index
    translucent = any(alpha < 0xFF for alpha in texture.rgba[3::4])
    return gltf_index, translucent


def add_model_graph(
    builder: GltfBuilder,
    data: bytes,
    name: str,
    items: tuple[fld.ModelItem, ...],
    assets: tuple[fld.ModelAsset, ...],
    draw_roots: dict[int, tuple[int, ...]],
    draw_lists: dict[int, fld.ModelDrawList],
    draws: dict[int, fld.ModelDraw],
    *,
    meters_per_unit: float,
    textures: tuple[tmx.Texture, ...] | None = None,
    texture_cache: dict[int, int] | None = None,
) -> tuple[tuple[int, ...], tuple[int, ...]]:
    """Append one decoded SDF model graph and return its nodes and roots."""

    if texture_cache is None:
        texture_cache = {}
    packet_cache: dict[tuple[int, int], tuple[fld.ModelMesh, ...]] = {}
    geometry_cache: dict[tuple[int, int, int], tuple[dict[str, int], int, int]] = {}
    mesh_cache: dict[int, int] = {}
    material_cache: dict[tuple[int, bool], int] = {}

    def material_for(
        asset_index: int, translucent_vertices: bool, has_texcoords: bool
    ) -> int:
        if textures is None:
            return 0
        key = asset_index, has_texcoords
        old = material_cache.get(key)
        if old is not None:
            if translucent_vertices:
                builder.document["materials"][old]["alphaMode"] = "BLEND"
            return old

        asset = assets[asset_index]
        fields = dict(asset.fields)
        material = {
            "name": f"{name}/asset_{asset_index}",
            "doubleSided": True,
            "pbrMetallicRoughness": {
                "metallicFactor": 0.0,
                "roughnessFactor": 1.0,
            },
            "extras": {"ddsAssetFlags": asset.flags},
        }
        translucent = translucent_vertices
        if "resource_04" in fields:
            source_index = int(fields["resource_04"][0])
            material["extras"]["ddsPrimaryTexture"] = source_index
            if source_index < 0 or source_index >= len(textures):
                raise fld.FldError(
                    f"model {name} asset {asset_index} references texture "
                    f"{source_index}, but the bundle has {len(textures)} textures"
                )
            if has_texcoords:
                texture_index, texture_translucent = _gltf_texture(
                    builder,
                    textures,
                    texture_cache,
                    source_index,
                    f"model {name} asset {asset_index}",
                )
                material["pbrMetallicRoughness"]["baseColorTexture"] = {
                    "index": texture_index
                }
                translucent |= texture_translucent
        if "resource_20" in fields:
            secondary, mode = fields["resource_20"]
            if secondary < 0 or secondary >= len(textures):
                raise fld.FldError(
                    f"model {name} asset {asset_index} references secondary "
                    f"texture {secondary}, but the bundle has {len(textures)} textures"
                )
            material["extras"]["ddsSecondaryTexture"] = int(secondary)
            material["extras"]["ddsSecondaryTextureMode"] = int(mode)
        if translucent:
            material["alphaMode"] = "BLEND"
        material_index = len(builder.document["materials"])
        builder.document["materials"].append(material)
        material_cache[key] = material_index
        return material_index

    def item_mesh(item: fld.ModelItem) -> int | None:
        if not item.commands:
            return None
        old = mesh_cache.get(item.commands)
        if old is not None:
            return old
        primitives = []
        for list_index, list_offset in enumerate(draw_roots[item.commands]):
            draw_list = draw_lists[list_offset]
            for draw_index, draw_offset in enumerate(draw_list.draws):
                draw = draws[draw_offset]
                packet_key = draw.packet, draw.quadwords * 0x10
                packet_meshes = packet_cache.get(packet_key)
                if packet_meshes is None:
                    packet_meshes, _ = fld._read_model_mesh_packet(
                        data,
                        packet_key[0],
                        packet_key[1],
                        f"model {name} packet",
                    )
                    packet_cache[packet_key] = packet_meshes
                for mesh_index, mesh in enumerate(packet_meshes):
                    geometry_key = draw.packet, draw.quadwords, mesh_index
                    geometry = geometry_cache.get(geometry_key)
                    if geometry is None:
                        geometry = _mesh_geometry(
                            builder,
                            mesh,
                            meters_per_unit,
                            f"model {name} packet 0x{draw.packet:x} mesh {mesh_index}",
                        )
                        geometry_cache[geometry_key] = geometry
                    attributes, indices, controls = geometry
                    translucent_vertices = bool(
                        mesh.colors
                        and any(color[3] < 0x80 for color in mesh.colors)
                    )
                    material_index = material_for(
                        draw.asset,
                        translucent_vertices,
                        mesh.texcoords is not None,
                    )
                    primitives.append(
                        {
                            "attributes": attributes,
                            "indices": indices,
                            "material": material_index,
                            "mode": 4,
                            "extras": {
                                "ddsAsset": draw.asset,
                                "ddsMaterialIndex": material_index,
                                "ddsDrawSelector": draw_list.selector,
                                "ddsDrawListIndex": list_index,
                                "ddsDrawIndex": draw_index,
                                "ddsPacketMeshIndex": mesh_index,
                                "ddsMeshControls": list(mesh.controls),
                                "ddsProgramAddress": mesh.program,
                                "ddsTriangleControlAccessor": controls,
                            },
                        }
                    )
        if not primitives:
            return None
        mesh_index = len(builder.document["meshes"])
        builder.document["meshes"].append(
            {"name": f"{name}/node_{item.node_id}", "primitives": primitives}
        )
        mesh_cache[item.commands] = mesh_index
        return mesh_index

    node_indices = []
    for item in items:
        node = {
            "name": f"{name}/node_{item.node_id}",
            "extras": {"ddsNodeId": item.node_id},
        }
        translation = item.position[:3]
        _set_transform_component(
            node,
            "translation",
            translation,
            [value * meters_per_unit for value in translation],
        )
        _set_transform_component(
            node,
            "rotation",
            item.rotation,
            _euler_quaternion(*item.rotation)
            if all(math.isfinite(value) for value in item.rotation)
            else [],
        )
        scale = item.scale[:3]
        _set_transform_component(node, "scale", scale, list(scale))
        if item.bounds:
            bounds = struct.unpack_from("<6f", data, item.bounds)
            if all(math.isfinite(value) for value in bounds):
                node["extras"]["ddsBounds"] = {
                    "minimum": [value * meters_per_unit for value in bounds[:3]],
                    "maximum": [value * meters_per_unit for value in bounds[3:]],
                }
            else:
                node["extras"]["ddsOmittedBoundsBits"] = _float_bits(bounds)
        mesh_index = item_mesh(item)
        if mesh_index is not None:
            node["mesh"] = mesh_index
        node_indices.append(len(builder.document["nodes"]))
        builder.document["nodes"].append(node)

    roots = []
    for item_index, item in enumerate(items):
        if item.parent < 0:
            roots.append(node_indices[item_index])
        else:
            parent = builder.document["nodes"][node_indices[item.parent]]
            parent.setdefault("children", []).append(node_indices[item_index])
    return tuple(node_indices), tuple(roots)


def build_gltf(
    data: bytes,
    *,
    textures: tuple[tmx.Texture, ...] | None = None,
    resources: set[str] | None = None,
    meters_per_unit: float = 1.0,
    frames_per_second: float = 1.0,
) -> tuple[dict, bytes]:
    """Return a glTF document and binary buffer for the selected FLD1 models."""

    if not math.isfinite(meters_per_unit) or meters_per_unit <= 0.0:
        raise fld.FldError("meters per unit must be a positive finite number")
    if not math.isfinite(frames_per_second) or frames_per_second <= 0.0:
        raise fld.FldError("frames per second must be a positive finite number")
    words, data_end, relocation_tuple = fld._read_header(data)
    if data[4:8] != b"FLD1":
        raise fld.FldError("model export requires an FLD1 file")
    relocations = set(relocation_tuple)
    rows = fld._read_types(data, words, data_end)
    field_resources = fld._read_resources(data, rows)
    builder = GltfBuilder.create()
    found: set[str] = set()
    exported = 0
    texture_cache: dict[int, int] = {}
    for resource in field_resources:
        if resource.type_id != 2 or not resource.data:
            continue
        name = (
            fld._fixed_string(data, resource.name, "model resource name")
            if resource.name
            else f"model_{resource.serial:04d}"
        )
        if resources is not None and name not in resources:
            continue
        found.add(name)
        model, items = fld._read_model_resource(
            data, resource.data, data_end, f"model {name}"
        )
        assets = fld._read_model_assets(
            data, model.assets, data_end, f"model {name} assets"
        )
        motion = fld._read_model_motion(
            data,
            model.motion,
            data_end,
            relocations,
            len(items),
            len(assets),
            f"model {name} motion",
        )
        draw_roots, draw_lists, draws = fld._read_model_draw_graph(
            data,
            items,
            len(assets),
            data_end,
            relocations,
            f"model {name}",
        )
        node_indices, roots = add_model_graph(
            builder,
            data,
            name,
            items,
            assets,
            draw_roots,
            draw_lists,
            draws,
            meters_per_unit=meters_per_unit,
            textures=textures,
            texture_cache=texture_cache,
        )
        wrapper = {
            "name": name,
            "children": roots,
            "extras": {
                "ddsResourceSerial": resource.serial,
                "ddsMotionBindings": [
                    {
                        "family": fld.MODEL_MOTION_FAMILIES[binding.family],
                        "selector": binding.selector,
                        "target": binding.target,
                        "format": fld.MODEL_MOTION_FORMATS[
                            binding.family, binding.selector
                        ][0],
                    }
                    for binding in motion.bindings
                ],
                "ddsMotionClips": [
                    _clip_summary(clip) for clip in motion.clips
                ],
            },
        }
        if resource.transform:
            values = struct.unpack_from("<12f", data, resource.transform)
            translation = values[0:3]
            _set_transform_component(
                wrapper,
                "translation",
                translation,
                [value * meters_per_unit for value in translation],
            )
            rotation = values[4:8]
            _set_transform_component(
                wrapper,
                "rotation",
                rotation,
                _normalized_quaternion(rotation)
                if all(math.isfinite(value) for value in rotation)
                else [],
            )
            scale = values[8:11]
            _set_transform_component(wrapper, "scale", scale, list(scale))
        wrapper_index = len(builder.document["nodes"])
        builder.document["nodes"].append(wrapper)
        builder.document["scenes"][0]["nodes"].append(wrapper_index)
        _add_animations(
            builder,
            data,
            name,
            motion,
            tuple(node_indices),
            meters_per_unit,
            frames_per_second,
        )
        exported += 1

    if resources is not None:
        missing = resources - found
        if missing:
            raise fld.FldError(f"model resources not found: {', '.join(sorted(missing))}")
    if not exported:
        raise fld.FldError("FLD1 contains no selected model resources")
    builder.binary.extend(bytes((-len(builder.binary)) & 3))
    builder.document["buffers"] = [{"byteLength": len(builder.binary)}]
    builder.document["asset"]["extras"] = {
        "ddsMetersPerUnit": meters_per_unit,
        "ddsFramesPerSecond": frames_per_second,
        "ddsNativeAxesPreserved": True,
    }
    return builder.document, bytes(builder.binary)


def encode_glb(document: dict, binary: bytes) -> bytes:
    json_data = json.dumps(
        document,
        ensure_ascii=False,
        allow_nan=False,
        separators=(",", ":"),
    ).encode("utf-8")
    json_data += b" " * ((-len(json_data)) & 3)
    binary += bytes((-len(binary)) & 3)
    length = 12 + 8 + len(json_data) + 8 + len(binary)
    return b"".join(
        (
            struct.pack("<4sII", b"glTF", 2, length),
            struct.pack("<II", len(json_data), GLB_JSON_CHUNK),
            json_data,
            struct.pack("<II", len(binary), GLB_BIN_CHUNK),
            binary,
        )
    )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument(
        "--resource",
        action="append",
        dest="resources",
        help="export only this model resource name (repeatable)",
    )
    parser.add_argument("--meters-per-unit", type=float, default=1.0)
    parser.add_argument("--frames-per-second", type=float, default=1.0)
    parser.add_argument(
        "--texture-bundle",
        type=Path,
        help="TBN/TXP0 bundle used by an FLD1 or FLD1 source",
    )
    args = parser.parse_args()
    try:
        textures = None
        if args.input.suffix.lower() == ".lb":
            if args.texture_bundle is not None:
                raise fld.FldError("an LB input already supplies its texture bundle")
            archive = lb.parse_archive(args.input.read_bytes())
            models = [entry for entry in archive.entries if entry.extension.upper() == "F1"]
            bundles = [entry for entry in archive.entries if entry.extension.upper() == "TBN"]
            if len(models) != 1 or len(bundles) != 1:
                raise fld.FldError(
                    "LB model export requires exactly one F1 and one TBN entry"
                )
            data = lb.entry_data(models[0])
            textures = tmx.parse_bundle(lb.entry_data(bundles[0]))
        elif args.input.suffix.lower() == ".f1asm":
            data = fld.encode(fld.parse_source(args.input.read_text(encoding="utf-8")))
        else:
            data = args.input.read_bytes()
        if args.texture_bundle is not None:
            textures = tmx.parse_bundle(args.texture_bundle.read_bytes())
        document, binary = build_gltf(
            data,
            textures=textures,
            resources=set(args.resources) if args.resources else None,
            meters_per_unit=args.meters_per_unit,
            frames_per_second=args.frames_per_second,
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(encode_glb(document, binary))
    except (OSError, fld.FldError, lb.LbError, tmx.TmxError, ValueError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
