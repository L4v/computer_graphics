/**
 * @file icosahedron.hpp
 * @author Jovan Ivosevic
 * @brief Icosahedron class
 * Some specific implementation credits: https://www.danielsieger.com/blog/2021/01/03/generating-platonic-solids.html
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 * 
 *
 */
#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Triangle {
	glm::vec3 V0;
	glm::vec3 V1;
	glm::vec3 V2;
};

/**
 * @brief A twenty sided polyhedra with each side being a triangle
 *
 *
 */
class Icosahedron {
public:

	/**
	 * @brief Ctor - sets up data
	 *
	 */
	Icosahedron();

	/**
	 * @brief Vertices getter
	 *
	 * @returns vertices (with only position values)
	 *
	 */
	std::vector<float> const &GetVertices() const;


	/**
	 * @brief Surface normals getter
	 *
	 * @returns Normals of triangle surfaces
	 */
	std::vector<float> const &GetSurfaceNormals() const;

	/**
	 * @brief Vertex normals getter
	 *
	 * @returns Normals for each vertex (more precise lighting calculations)
	 *
	 */
	std::vector<float> const &GetVertexNormals() const;

	/**
	 * @brief Debug surface normals getter
	 *
	 * @returns lines (two points each) in direction of surface normals
	 *
	 */
	std::vector<float> const& GetDebugSurfaceNormals() const;

	/**
	 * @brief Debug vertex normals getter
	 *
	 * @returns lines (two points each) in direction of vertex normals
	 *
	 */
	std::vector<float> const& GetDebugVertexNormals() const;
private:
	std::vector<float> mSurfaceNormals;
	std::vector<float> mVertexNormals;
	std::vector<float> mDebugSurfaceNormals;
	std::vector<float> mDebugVertexNormals;
	std::vector<float> mVertices;

	/**
	 * @brief Generates 12 primordial points of the icosahedron
	 *
	 * @param points - Out vector for points
	 *
	 */
	void generateIcosahedronPoints(std::vector<glm::vec3>& points);

	/**
	 * @brief Generates 20 triangles of the icosahedron
	 *
	 * @param points - icosahedral points
	 * @param triangles - triangles out vector
	 *
	 */
	void generateIcosahedronTriangles(const std::vector<glm::vec3>& points, std::vector<Triangle>& triangles);

	/**
	 * @brief Generates vertex data for OpenGL out of triangles
	 *
	 * @param triangles - icosahedral triangles
	 * @param vertices - vertex out vector
	 *
	 */
	void generateIcosahedronVertices(const std::vector<Triangle>& triangles, std::vector<float>& vertices);

	/**
	 * @brief Generates Normals for surface
	 *
	 * @param icosahedron triangles - normals out vector
	 * @param normals - normals out vector out vector
	 *
	 */
	void generateIcosahedronSurfaceNormals(const std::vector<Triangle>& triangles, std::vector<float>& normals);

	/**
	 * @brief Generates Normals for each vertex
	 *
	 * @param triangles icosahedron triangles 
	 * @param normals - normals out vector out vector
	 *
	 */
	void generateIcosahedronVertexNormals(const std::vector<Triangle>& triangles, std::vector<float>& normals);

	/**
	 * @brief Generates Debug Normals for each surface
	 *
	 * @param triangles icosahedron triangles
	 * @param normals - normals out vector out vector
	 *
	 */
	void generateIcosahedronDebugSurfaceNormals(const std::vector<Triangle>& triangles, std::vector<float>& normals);

	/**
	 * @brief Generates Debug Normals for each vertex
	 *
	 * @param triangles icosahedron triangles
	 * @param normals - normals out vector out vector
	 *
	 */
	void generateIcosahedronDebugVertexNormals(const std::vector<Triangle>& triangles, std::vector<float>& normals);
};