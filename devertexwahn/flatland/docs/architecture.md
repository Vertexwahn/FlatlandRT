# Architecture

# Architecture

```mermaid
sequenceDiagram
    Renderer->>Sensor: generate_ray
    Renderer->>Integrator: trace_ray
    Integrator->>Scene: intersect
    Scene->>Intersector: intersect
    Intersector-->>Scene: :MediumEvent
    Scene-->>Integrator: :MediumEvent
    Integrator-->>Renderer: :Color
```

![](sqd_rendering.svg)
