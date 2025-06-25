# Tone Mapper

```shell
bazel run --config=gcc13 //imaging/tone_mapping:tone_mapper -- --version
```

```shell
bazel run --config=gcc13 //imaging/tone_mapping:tone_mapper -- --help
```

```shell
for SAMPLES in {4..100}
do
    bazel run --config=gcc13 //imaging/tone_mapping:tone_mapper -- \
    --filename=${HOME}/dev/Piper/devertexwahn/okapi/scenes/automotive_paint_shader/frame/frame_${SAMPLES}.exr \
    --outfile=${HOME}/dev/Piper/devertexwahn/okapi/scenes/automotive_paint_shader/frame/frame_${SAMPLES}.png
done
```
