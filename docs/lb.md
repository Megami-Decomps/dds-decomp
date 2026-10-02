# LB field archives

`tools/lb.py` reads and rebuilds the `.LB` resource archives used by DDS and
Nocturne. The tracked sources reconstruct all 31 DDS1 and 25 DDS2 shared field
archives from authored INF, NPL, SKY, WAP, BF, and AMB resources. The
`f011_001.lbasm` sources also place rebuilt FLD2 and FLD1 resources back beside
the area's retained texture resource. Run:

```sh
python3 tools/extract.py
python3 configure.py
ninja dds1-field-archives dds2-field-archives
```

The first command extracts the required retail base archives from a disc image
the user supplies. The repository contains their structure, resource hashes,
and replacement sources; it does not contain retail archive payloads. Every
tracked output rebuilds to its retail SHA-1 value.

## Container

An LB archive is an ordered list of resource blocks. Every block begins on a
`0x40` boundary and has this 16-byte header:

| Offset | Type | Meaning |
|---:|---|---|
| `0x00` | u8 | resource type; ordinary DDS field entries use `1` |
| `0x01` | u8 | stored payload is compressed |
| `0x02` | s16 | user id |
| `0x04` | u32 | header plus stored-payload size |
| `0x08` | char[4] | resource extension, such as `TBN`, `F2`, or `F1` |
| `0x0c` | u32 | decompressed size; an uncompressed entry may use zero |

The final aligned block has type `0xff` and extension `END0`. There is no
central directory: readers advance by the stored size and round up to the next
`0x40` boundary. DDS1 `f025_006.LB` uses the zero-size convention for its
uncompressed TBN entry; its stored payload length is the effective raw size.

## Compression

The compressed stream uses a one-byte command whose upper three bits select
an operation and whose lower five bits hold the count. A zero count is followed
by a little-endian u16 count. `0xff` ends the stream.

| Opcode | Operation | Extra bytes |
|---:|---|---|
| `0` | copy literal bytes | `count` bytes |
| `1` | emit zero bytes | none |
| `2` | repeat one byte | one byte |
| `3` | copy from an output distance | u8 distance |
| `4` | copy from an output distance | u16 distance |
| `5` | copy bytes with a zero after each | `count` bytes |

Distance copies may overlap, as in an ordinary LZ stream. The decoder checks
every input boundary, distance, declared output size, and terminator.

The assembler has two deliberate modes. If a rebuilt resource is identical to
the matching resource in the supplied base archive, it retains the original
encoded block and padding byte-for-byte. If the resource changed, it uses a
deterministic bounded-search compressor and emits a fresh block. This gives an
exact retail build for unchanged source without depending on the retail
encoder's private match-selection heuristic, while edited source remains a
complete compressed archive.

## Source format

An `.lbasm` file declares the expected base archive and each entry in order:

```text
lb 1
base sha1=3937f9b1d070d83657220806c3abd6a90b21e4b4 size=0xf5ac0
entry type=1 compressed=1 user=0 extension=TBN raw_size=0xc0780 retail_sha1=9cd61610692ef06b49476f3568f4608db3d23866 source=base
entry type=1 compressed=1 user=0 extension=F2 raw_size=0x659f retail_sha1=48fdfb28d35f6d61d1d759f463ec1e750b967a9b source=data/field/f011_001.f2
entry type=1 compressed=1 user=0 extension=F1 raw_size=0xbb747 retail_sha1=f4ade60ad1620927f43f60eb2be55a2272ac2cc8 source=data/field/f011_001.f1
```

`source=base` retains a resource that has not yet been reconstructed.
Any other relative name is loaded from `--resources`; the normal build points
that directory at `build/<version>`. This lets an archive refer to field data
as `data/field/f011.inf` and scripts as `scripts/field/f011.bf`. The build reads
these declarations to create its dependency graph, so changing any named
resource rebuilds every archive that contains it.

The complete shared-archive corpus can also be checked directly against an
owned disc. The tool identifies each entry by the exact rebuilt resource,
checks every generated source in memory, and compares the tracked archive and
checksum manifests:

```sh
python3 tools/field_archive_corpus.py dds1 /path/to/game.iso --resources build/dds1
python3 tools/field_archive_corpus.py dds2 /path/to/game.iso --resources build/dds2
```

Useful standalone commands are:

```sh
python3 tools/lb.py disassemble orig/dds1/field/f011_001.LB
python3 tools/lb.py extract orig/dds1/field/f011_001.LB out/f011_001
python3 tools/lb.py verify --resources build/dds1 \
  src/dds1/data/field/f011_001.lbasm \
  orig/dds1/field/f011_001.LB
```
