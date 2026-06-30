# Ray Tracer — Theory Notes

---

## 1. Ray-Sphere Intersection

### Sphere (mathematical definition)
All points `P` exactly `r` distance from center `C`:
```
|P - C|² = r²
```

### Ray (mathematical definition)
A point moving along a direction over time `t`:
```
P(t) = origin + t * direction
```

### Intersection — the core idea
Substitute the ray into the sphere equation to find `t` values where the ray lies ON the sphere:
```
|P(t) - C|² = r²
```

Let `oc = origin - C`, expand using dot product rules:
```
t²(direction·direction) + 2t(oc·direction) + (oc·oc) - r² = 0
```

This is a quadratic — `at² + bt + c = 0` where:
```
a = direction · direction
b = 2 * (oc · direction)
c = (oc · oc) - r²
```

### Discriminant
```
discriminant = b² - 4ac
```
| Result | Meaning |
|---|---|
| `discriminant < 0` | Ray misses the sphere |
| `discriminant = 0` | Ray grazes the sphere (1 hit) |
| `discriminant > 0` | Ray enters and exits the sphere (2 hits) |

### Solving for t
```
t = (-b ± √discriminant) / 2a
```

Pick the **smallest positive t** — that's the closest hit in front of the camera.

### Optimized form (b/2 trick)
Since `b` always has a factor of 2, substitute `h = b/2`:
```
discriminant = h² - ac
t = (-h ± √discriminant) / a
```

### Sign of t
| t value | Meaning |
|---|---|
| `t > 0` | Hit is in front of the camera ✅ |
| `t < 0` | Sphere is behind the camera — reject ❌ |
| Both roots negative | Ray would have to travel backwards to hit — not a valid hit |

---

## 2. Hittable Interface

`Hittable` is a pure abstract interface — a contract that says:
> "Any object in the scene must be able to answer: does this ray hit you?"

Concrete shapes (`Sphere`, `Triangle`, etc.) inherit from it and implement `hit()` in their own way. The `Scene` holds a list of `Hittable*` and calls `hit()` on each without caring what shape it is.

```
Hittable  (interface)
   ↓
Sphere   → implements hit() using quadratic sphere-ray intersection
Triangle → implements hit() using Möller–Trumbore algorithm (later)
```

### HitRecord
Stores the result of a successful hit:
| Field | Meaning |
|---|---|
| `point` | World-space position of the hit |
| `normal` | Surface normal at the hit point |
| `t` | Distance along the ray |
| `frontFace` | Whether the ray hit the outside of the surface |

### tMin / tMax
Every `hit()` call takes a valid range `[tMin, tMax]`:
- Rejects hits too close (avoids self-intersection / shadow acne later)
- The Scene narrows `tMax` to `closestSoFar` as it finds hits — ensures only the nearest intersection survives

---

## 3. Scene

The Scene owns all objects in the world via `unique_ptr<Hittable>`.

- `unique_ptr` = one clear owner, object dies when scene dies
- `shared_ptr` = multiple owners — not needed here

### How Scene::hit() works
```
for each object:
    if object->hit(ray, tMin, closestSoFar, tempRecord):
        closestSoFar = tempRecord.t   ← narrow the window
        record = tempRecord
```
By narrowing `closestSoFar` each iteration, only the nearest hit survives at the end.

---

## 4. (coming soon...)

## 4. Materials

A material defines how a ray interacts with a surface. When a ray hits an object, the material decides:
- Does the ray scatter? If so, in what direction?
- How much is the color attenuated (absorbed)?

The `Material` interface has one pure virtual method:
```
scatter(rayIn, hitRecord, attenuation, scattered) → bool
```
Returns `true` if the ray scatters, `false` if it's fully absorbed.

### Albedo
The base color of a material. Represents how much of each RGB channel is reflected. `(1,1,1)` = white (reflects everything), `(0,0,0)` = black (absorbs everything).

---

## 5. Lambertian (Diffuse)

Models perfectly matte surfaces — light scatters in random directions regardless of incoming angle.

### Scatter direction
```
scatterDirection = hitNormal + randomUnitVector()
```
Adding a random unit vector to the normal biases scattering away from the surface (hemisphere-weighted toward the normal) — this is **Lambertian distribution**.

### Degenerate case
If `randomUnitVector()` is exactly opposite to the normal, `scatterDirection` becomes zero. Always check `nearZero()` and fall back to the normal itself.

---

## 6. Path Tracing

Each ray that hits a surface spawns a new **scattered ray** which is traced recursively. The final color is the product of all attenuations along the bounce chain.

```
color = attenuation1 * attenuation2 * attenuation3 * ... * skyColor
```

### Max depth
Recursion is capped at `maxRayDepth`. If a ray exceeds the depth limit, return black — it has bounced too many times to contribute meaningful light.

### tMin = 0.001 (shadow acne fix)
When a scattered ray is spawned at a hit point, floating point error can place it slightly inside the surface, causing it to immediately hit itself. Setting `tMin = 0.001` rejects these self-intersections.

---

## 7. Gamma Correction

Monitors display colors in gamma space, but ray tracers compute in **linear light space**. Without correction, renders look too dark.

### Gamma 2 correction (approximation)
```
displayColor = sqrt(linearColor)
```
Applied per channel in `FrameBuffer::setPixel()` before storing.

---

## 8. Anti-Aliasing

Each pixel fires multiple rays with **sub-pixel random offsets** instead of one ray through the pixel center. The results are averaged.

```
u = (x + randomFloat()) / (width  - 1)
v = (y + randomFloat()) / (height - 1)
```

Smooths jagged edges (aliasing) by blending colors at boundaries.

---

## 9. Progressive Rendering

Instead of blocking until all samples are done, render one sample per pixel per pass and display after each pass.

### Welford running average
Updates the average incrementally without storing all samples:
```
newAverage = oldAverage + (newSample - oldAverage) / passNumber
```

### AccumulationBuffer
Stores raw running averages separate from `FrameBuffer`. `FrameBuffer` only receives final display-ready colors after each full pass.

---

## 10. Tile Rendering

The image is split into 32x32 pixel tiles. Each tile is an independent unit of work.

- Better cache performance — spatially close pixels share ray data
- Foundation for multithreading — tiles are handed out to threads independently
- `TileScheduler` manages the tile queue using `std::atomic<int>` index — no mutex needed for tile distribution

### TileScheduler::getNextTile()
Uses `fetch_add` to atomically grab the next tile index. Returns `std::nullopt` when queue is empty — threads use this as their stop signal.

---

## 11. Multithreading

`renderPass()` spawns `N = hardware_concurrency()` threads. Each thread loops calling `getNextTile()` until it gets `std::nullopt`.

### Why no mutex on AccumulationBuffer?
Tiles never overlap — each pixel is owned by exactly one tile, so no two threads ever write the same pixel simultaneously.

### frameMutex
Only the final `FrameBuffer` flush needs a mutex — the main thread reads it for `present()` while the render thread writes it after each pass.

---

## 12. Metal (coming soon...)

## 12. Metal (Specular Reflection)

Models shiny metallic surfaces using perfect specular reflection.

### Reflect formula
```
reflected = v - 2(v·n)n
```
Flips the component of the incoming ray `v` that points into the surface. Lives in `Vector3::reflect()` since it's pure math reusable by any material.

### Fuzz
Simulates microscopic surface imperfections by nudging the reflected ray:
```
reflected = reflect(v, n).normalized() + fuzz * randomUnitVector()
```
- `fuzz = 0` → perfect mirror
- `fuzz = 1` → very blurry reflection
- Normalize before adding fuzz so fuzz scale is consistent regardless of ray length

### Absorption case
If the fuzzed reflected ray points INTO the surface (`reflected·normal <= 0`), absorb it — return false from scatter. Physically: the ray would travel through the solid metal which is wrong.

---

## 13. Glass (Dielectrics)

Models transparent materials that both reflect and refract light.

### Index of Refraction (IOR)
```
Air     → 1.0
Glass   → 1.5
Water   → 1.33
Diamond → 2.4
```

### Snell's Law
```
η₁ · sin(θ₁) = η₂ · sin(θ₂)
```
The ratio `η₁/η₂` (etaRatio) determines how much the ray bends at the surface boundary.

### Entering vs exiting
Use `HitRecord::frontFace` to determine ray direction through surface:
```
frontFace = true  → entering glass → etaRatio = 1.0 / ior
frontFace = false → exiting glass  → etaRatio = ior
```

### Total Internal Reflection
When `etaRatio * sinTheta > 1.0` there's no real solution to Snell's Law — ray must reflect instead of refract. Common inside dense materials at steep angles.

### Schlick Approximation (Fresnel)
At glancing angles, glass reflects more than it refracts. Schlick approximates this:
```
R₀ = ((η₁ - η₂) / (η₁ + η₂))²
R(θ) = R₀ + (1 - R₀)(1 - cosθ)⁵
```
Randomly choose reflect vs refract based on this probability each ray.

### Attenuation
Perfect glass → `(1, 1, 1)` — doesn't tint light at all.

### Hollow glass sphere trick
Place a negative radius sphere inside a glass sphere — negative radius flips normals, creating an air bubble inside glass.

---

## 14. Emissive

The simplest material — emits light, never scatters.

```
scatter() → always returns false
emitted() → returns color * strength
```

### Integration with path tracer
`Material` base class has a default `emitted()` returning black. Only `Emissive` overrides it. The integrator always calls `emitted()` BEFORE trying to scatter:

```
emitted = material->emitted()       // black for non-emissive
if scatter succeeds:
    return emitted + attenuation * trace(scattered)
else:
    return emitted                  // pure light or absorbed
```

This ensures emissive surfaces return their color even though scatter returns false.

### Strength
Multiplier above 1.0 makes the light brighter than white — needed since path tracers don't clamp intermediate values, only the final pixel.

## 15. AABB (Axis-Aligned Bounding Box)

A box whose faces are always parallel to the XY, XZ, YZ planes. Defined by two corners — `minimum` and `maximum`.

### Why axis-aligned?
Much cheaper to intersect than an arbitrary oriented box — only 6 comparisons per ray test.

### Slab method
For each axis, compute when the ray enters and exits that axis's slab:
```
t_enter = (min[axis] - origin[axis]) / direction[axis]
t_exit  = (max[axis] - origin[axis]) / direction[axis]
```
The ray hits the box only if all three slabs overlap:
```
tEnter = max(tx_enter, ty_enter, tz_enter)
tExit  = min(tx_exit,  ty_exit,  tz_exit)
hit = tEnter < tExit
```
Swap t0/t1 if direction is negative (ray going in reverse along that axis).

### expand()
Merges two AABBs into one that contains both — used during BVH construction to compute parent node boxes.

---

## 16. BVH (Bounding Volume Hierarchy)

Organizes scene objects into a binary tree of bounding boxes. Turns O(n) ray-scene intersection into O(log n).

```
BVHNode (root — whole scene box)
├── BVHNode (left box)
│   ├── Sphere1
│   └── Sphere2
└── BVHNode (right box)
    ├── Sphere3
    └── Sphere4
```

Ray misses left box → skips Sphere1 and Sphere2 entirely.

### Why BVHNode is a Hittable
Makes tree traversal recursive with no special cases. The root node drops into Scene just like any other object. Each child is also a Hittable — same `hit()` call all the way down to leaf primitives.

### Construction (midpoint)
1. Compute combined AABB of all objects in range
2. Pick longest axis (x, y, or z) of the box extent
3. Sort objects by centroid along that axis
4. Split at midpoint — recurse left and right halves

### hit() traversal
```
if ray misses bounding box → return false
test left child
test right child (narrow tMax to record.t if left hit)
return left || right
```
Narrowing tMax on the right test ensures only the closer hit survives.

### Every Hittable needs boundingBox()
Added as pure virtual to `Hittable`. Sphere returns:
```
min = center - Vector3(r, r, r)
max = center + Vector3(r, r, r)
```

---

## 17. SAH BVH (Surface Area Heuristic)

Replaces midpoint split with a cost-optimal split. Used when object count > 4.

### Core idea
Probability of a ray hitting a box ∝ its surface area. A large child box is more likely to be tested — it should contain fewer objects to keep cost low.

### Cost formula
```
cost = 2 * T_box
     + (AL/AP) * nL * T_prim
     + (AR/AP) * nR * T_prim
```
- `AP` = parent surface area
- `AL`, `AR` = left/right child surface areas
- `nL`, `nR` = object counts
- `T_box = 1`, `T_prim = 2` (typical constants)

### Bucketing (O(n·B) instead of O(n²))
Divide each axis into B=12 buckets. Assign each object to a bucket by centroid. Test B-1 split positions per axis. Pick minimum cost split across all three axes.

### Fallback cases
- count == 1 → both children point to same object
- count == 2 → direct assignment, no recursion
- count <= 4 → naive midpoint split (SAH overhead not worth it)
- SAH partition degenerates → fall back to midpoint

### surfaceArea() helper
```
e = max - min
SA = 2 * (e.x*e.y + e.y*e.z + e.z*e.x)
```

---

## 18. Textures

Instead of a flat `Vector3 albedo`, materials hold a `shared_ptr<Texture>` that returns a color given surface coordinates.

### Texture interface
```
sample(u, v, point) → Vector3
```
- `u`, `v` — normalized 0-1 UV coordinates
- `point` — 3D world-space hit point (for procedural textures)

### SolidColor
Always returns the same color regardless of u, v, point. Used as a drop-in replacement for flat albedo — `Lambertian(Vector3)` wraps it automatically.

### Checkerboard
Uses 3D point (not UV) for consistency across any surface:
```
sines = sin(scale*x) * sin(scale*y) * sin(scale*z)
sines < 0 → odd color, sines >= 0 → even color
```
Using 3D point means the pattern is consistent when seen through glass or reflected in metal.

### UV mapping on Sphere
Converts hit point on unit sphere to UV via spherical coordinates:
```
θ = acos(-y)           → latitude
φ = atan2(-z, x) + π   → longitude
u = φ / 2π
v = θ / π
```
Stored in `HitRecord.u` and `HitRecord.v`, filled by `Sphere::hit()`.

### Lambertian texture upgrade
`Lambertian` now holds `shared_ptr<Texture>` instead of `Vector3`.
Convenience constructor wraps a `Vector3` in `SolidColor` automatically — all existing code works unchanged.
`scatter()` calls `albedo->sample(u, v, point)` instead of using albedo directly.

---

## 19. Triangle Meshes and OBJ Loading

OBJ files store indexed vertex positions (`v`), texture coordinates (`vt`),
normals (`vn`), and polygon faces (`f`). `OBJLoader` reads those arrays,
resolves each face vertex, and triangulates polygons using a fan from the first
vertex.

Triangles use the Möller–Trumbore algorithm. Its two barycentric coordinates,
along with `w = 1 - u - v`, interpolate vertex normals and texture coordinates
at the exact hit point. A mesh builds its own BVH over the resulting triangles,
then behaves like one `Hittable` in the scene BVH.

---

## 20. Image Textures

`ImageTexture` uses `stb_image` to decode common formats such as PNG and JPEG.
The decoded RGB bytes are copied into a `std::vector`, so the texture owns its
pixel memory safely.

### UV to pixel coordinates

UV coordinates are clamped to `[0, 1]`, then converted to integer image
coordinates:

```
x = min(int(u * width), width - 1)
y = min(int((1 - v) * height), height - 1)
```

The `1 - v` flip is needed because OBJ UVs start at the bottom while image rows
are stored from the top. Sampling currently uses nearest-neighbor lookup.

### Color space

PNG and JPEG color values are normally stored in sRGB space. The path tracer
does lighting in linear space, so each sampled channel is converted from sRGB
to linear before it is returned. The framebuffer later applies display gamma
when presenting the final image.

If image loading fails, the texture returns bright magenta so missing assets
are immediately visible.

---

## 21. (coming soon — HDRI, DOF, Motion Blur)
