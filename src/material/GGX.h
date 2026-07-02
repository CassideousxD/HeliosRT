#pragma once
#include "math/Vector3.h"
#include <cmath>
#include <algorithm>

namespace GGX
{
    // Normal Distribution Function
    inline float distribution(float nDotH, float alpha)
    {
        float alpha2 = alpha * alpha;
        float denom = (nDotH * nDotH) * (alpha2 - 1.0f) + 1.0f;
        denom = 3.14159265f * denom * denom;
        return alpha2 / std::max(denom, 1e-8f);
    }

    // Smith geometry term (combined, Schlick-GGX approximation)
    inline float geometrySchlickGGX(float nDotX, float alpha)
    {
        float k = alpha * 0.5f;
        return nDotX / (nDotX * (1.0f - k) + k);
    }

    inline float geometry(float nDotV, float nDotL, float alpha)
    {
        return geometrySchlickGGX(nDotV, alpha) * geometrySchlickGGX(nDotL, alpha);
    }

    // Fresnel-Schlick with a color F0 (for metals) or scalar F0 (for dielectrics)
    inline Vector3 fresnelSchlick(float cosTheta, const Vector3& f0)
    {
        float t = std::pow(std::clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
        return f0 + (Vector3(1.0f,1.0f,1.0f) - f0) * t;
    }

    // Importance sample a microfacet normal from the GGX distribution
    // returns a half-vector in world space, built from local (n,t,b) frame
    inline Vector3 sampleHalfVector(
        const Vector3& normal, float alpha, float u1, float u2)
    {
        float phi = 2.0f * 3.14159265f * u1;
        float cosTheta = std::sqrt((1.0f - u2) / (1.0f + (alpha*alpha - 1.0f) * u2));
        float sinTheta = std::sqrt(std::max(0.0f, 1.0f - cosTheta*cosTheta));

        Vector3 localH(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);

        // build orthonormal basis around normal
        Vector3 up = (std::fabs(normal.z) < 0.999f) ? Vector3(0,0,1) : Vector3(1,0,0);
        Vector3 tangent = up.cross(normal).normalized();
        Vector3 bitangent = normal.cross(tangent);

        return (tangent * localH.x + bitangent * localH.y + normal * localH.z).normalized();
    }

    // pdf of the sampled half-vector, converted to pdf w.r.t outgoing direction
    inline float pdfHalfVector(float nDotH, float vDotH, float alpha)
    {
        float d = distribution(nDotH, alpha);
        return (d * nDotH) / std::max(4.0f * vDotH, 1e-8f);
    }
}