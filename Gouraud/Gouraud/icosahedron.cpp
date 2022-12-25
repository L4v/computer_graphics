#include "icosahedron.hpp"

glm::vec3
GetTriangleNormal(const Triangle& triangle) {
    glm::vec3 U = triangle.V1 - triangle.V0;
    glm::vec3 V = triangle.V2 - triangle.V0;
    return glm::vec3(
        (U.y * V.z) - (U.z * V.y),
        (U.z * V.x) - (U.x * V.z),
        (U.x * V.y) - (U.y * V.x)
    );
}

glm::vec3
GetTriangleCentroid(const Triangle& triangle) {
    float Cx = (triangle.V0.x + triangle.V1.x + triangle.V2.x) / 3.0f;
    float Cy = (triangle.V0.y + triangle.V1.y + triangle.V2.y) / 3.0f;
    float Cz = (triangle.V0.z + triangle.V1.z + triangle.V2.z) / 3.0f;
    return glm::vec3(Cx, Cy, Cz);
}

Icosahedron::Icosahedron() {
    std::vector<glm::vec3> Points;
    generateIcosahedronPoints(Points);
    std::vector<Triangle> Triangles;
    generateIcosahedronTriangles(Points, Triangles);
    generateIcosahedronVertices(Triangles, mVertices);
    generateIcosahedronSurfaceNormals(Triangles, mSurfaceNormals);
    // NOTE(Jovan): Same as getting just vertices
    generateIcosahedronVertexNormals(Triangles, mVertexNormals);

    generateIcosahedronDebugSurfaceNormals(Triangles, mDebugSurfaceNormals);
    generateIcosahedronDebugVertexNormals(Triangles, mDebugVertexNormals);
}

std::vector<float> const
& Icosahedron::GetVertices() const {
    return mVertices;
}

std::vector<float> const
& Icosahedron::GetSurfaceNormals() const {
    return mSurfaceNormals;
}

std::vector<float> const
& Icosahedron::GetVertexNormals() const {
    return mVertexNormals;
}

std::vector<float> const
& Icosahedron::GetDebugSurfaceNormals() const {
    return mDebugSurfaceNormals;
}

std::vector<float> const
& Icosahedron::GetDebugVertexNormals() const {
    return mDebugVertexNormals;
}

void
Icosahedron::generateIcosahedronSurfaceNormals(const std::vector<Triangle>& triangles, std::vector<float>& normals) {
    for (const Triangle& t : triangles) {
        glm::vec3 Normal = 2.0f * GetTriangleNormal(t);
        // NOTE(Jovan): For each vertex use same normal
        for (unsigned i = 0; i < 3; ++i) {
            normals.push_back(Normal.x);
            normals.push_back(Normal.y);
            normals.push_back(Normal.z);
        }
    }
}

void
Icosahedron::generateIcosahedronVertexNormals(const std::vector<Triangle>& triangles, std::vector<float>& normals) {
    for (const Triangle& T : triangles) {
        normals.push_back(T.V0.x);
        normals.push_back(T.V0.y);
        normals.push_back(T.V0.z);
        normals.push_back(T.V1.x);
        normals.push_back(T.V1.y);
        normals.push_back(T.V1.z);
        normals.push_back(T.V2.x);
        normals.push_back(T.V2.y);
        normals.push_back(T.V2.z);
    }
}

void
Icosahedron::generateIcosahedronPoints(std::vector<glm::vec3>& points) {
    float Phi = (1.0f + sqrt(5.0f)) * 0.5f;
    float A = 1.0f;
    float B = 1.0f / Phi;
    points.push_back({ 0, B, -A });
    points.push_back({ B, A, 0 });
    points.push_back({ -B, A, 0 });
    points.push_back({ 0, B, A });
    points.push_back({ 0, -B, A });
    points.push_back({ -A, 0, B });
    points.push_back({ 0, -B, -A });
    points.push_back({ A, 0, -B });
    points.push_back({ A, 0, B });
    points.push_back({ -A, 0, -B });
    points.push_back({ B, -A, 0 });
    points.push_back({ -B, -A, 0 });
}

void
Icosahedron::generateIcosahedronTriangles(const std::vector<glm::vec3>& points, std::vector<Triangle>& triangles) {
    triangles.push_back({ points[2], points[1], points[0] });
    triangles.push_back({ points[1], points[2], points[3] });
    triangles.push_back({ points[5], points[4], points[3] });
    triangles.push_back({ points[4], points[8], points[3] });
    triangles.push_back({ points[7], points[6], points[0] });
    triangles.push_back({ points[6], points[9], points[0] });
    triangles.push_back({ points[11], points[10], points[4] });
    triangles.push_back({ points[10], points[11], points[6] });
    triangles.push_back({ points[9], points[5], points[2] });
    triangles.push_back({ points[5], points[9], points[11] });
    triangles.push_back({ points[8], points[7], points[1] });
    triangles.push_back({ points[7], points[8], points[10] });
    triangles.push_back({ points[2], points[5], points[3] });
    triangles.push_back({ points[8], points[1], points[3] });
    triangles.push_back({ points[9], points[2], points[0] });
    triangles.push_back({ points[1], points[7], points[0] });
    triangles.push_back({ points[11], points[9], points[6] });
    triangles.push_back({ points[7], points[10], points[6] });
    triangles.push_back({ points[5], points[11], points[4] });
    triangles.push_back({ points[10], points[8], points[4] });
}

void
Icosahedron::generateIcosahedronVertices(const std::vector<Triangle>& triangles, std::vector<float>& vertices) {
    for (const Triangle& T : triangles) {
        vertices.push_back(T.V0.x);
        vertices.push_back(T.V0.y);
        vertices.push_back(T.V0.z);
        vertices.push_back(T.V1.x);
        vertices.push_back(T.V1.y);
        vertices.push_back(T.V1.z);
        vertices.push_back(T.V2.x);
        vertices.push_back(T.V2.y);
        vertices.push_back(T.V2.z);
    }
}

void
Icosahedron::generateIcosahedronDebugSurfaceNormals(const std::vector<Triangle>& triangles, std::vector<float>& normals) {
    const float Length = 2.0f;
    // NOTE(Jovan): Generate lines (2 points) from center of triangle surface
    for (const Triangle& T : triangles) {
        glm::vec3 Centroid = GetTriangleCentroid(T);
        glm::vec3 TipOfNormal = Length * GetTriangleNormal(T);
        normals.push_back(Centroid.x);
        normals.push_back(Centroid.y);
        normals.push_back(Centroid.z);

        normals.push_back(TipOfNormal.x);
        normals.push_back(TipOfNormal.y);
        normals.push_back(TipOfNormal.z);
    }
}
void
Icosahedron::generateIcosahedronDebugVertexNormals(const std::vector<Triangle>& triangles, std::vector<float>& normals) {
    const float Length = 2.0f;
    // NOTE(Jovan): Generate lines (2 points) from each vertex
    for (const Triangle& T : triangles) {
        normals.push_back(T.V0.x);
        normals.push_back(T.V0.y);
        normals.push_back(T.V0.z);
        normals.push_back(Length * T.V0.x);
        normals.push_back(Length * T.V0.y);
        normals.push_back(Length * T.V0.z);

        normals.push_back(T.V1.x);
        normals.push_back(T.V1.y);
        normals.push_back(T.V1.z);
        normals.push_back(Length * T.V1.x);
        normals.push_back(Length * T.V1.y);
        normals.push_back(Length * T.V1.z);

        normals.push_back(T.V2.x);
        normals.push_back(T.V2.y);
        normals.push_back(T.V2.z);
        normals.push_back(Length * T.V2.x);
        normals.push_back(Length * T.V2.y);
        normals.push_back(Length * T.V2.z);
    }
}