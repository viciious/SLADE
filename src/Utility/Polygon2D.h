#pragma once

class GLTexture;
class MapSector;
class Polygon2D
{
public:
	struct Vertex
	{
		float x, y, z;
		float tx = 0.f, ty = 0.f;
		Vertex(float x = 0.f, float y = 0.f, float z = 0.f) : x{ x }, y{ y }, z{ z } {}
	};

	struct SubPoly
	{
		vector<Vertex> vertices;
		unsigned       vbo_offset = 0;
		unsigned       vbo_index  = 0;
	};

	Polygon2D()  = default;
	~Polygon2D() = default;

	GLTexture* getTexture() const { return texture_; }
	float      colRed() { return colour_[0]; }
	float      colGreen() { return colour_[1]; }
	float      colBlue() { return colour_[2]; }
	float      colAlpha() { return colour_[3]; }

	void setTexture(GLTexture* tex) { texture_ = tex; }
	void setColour(float r, float g, float b, float a);
	bool hasPolygon() const { return !sub_polys_.empty(); }
	int  vboUpdate() const { return vbo_update_; }
	void setZ(float z);
	void setZ(Plane plane);

	unsigned nSubPolys() const { return sub_polys_.size(); }
	void     addSubPoly();
	SubPoly* getSubPoly(unsigned index);
	void     removeSubPoly(unsigned index);
	void     clear();
	unsigned totalVertices();

	bool openSector(MapSector* sector);
	void updateTextureCoords(
		double scale_x  = 1,
		double scale_y  = 1,
		double offset_x = 0,
		double offset_y = 0,
		double rotation = 0);

	unsigned vboDataSize();
	unsigned writeToVBO(unsigned offset, unsigned index);
	void     updateVBOData();

	void render();
	void renderWireframe();
	void renderVBO(bool colour = true);
	void renderWireframeVBO(bool colour = true);

	static void setupVBOPointers();

private:
	// Polygon data
	vector<SubPoly> sub_polys_;
	GLTexture*      texture_   = nullptr;
	float           colour_[4] = { 1.f, 1.f, 1.f, 1.f };

	int vbo_update_ = 2;
};


class PolygonSplitter
{
	friend class Polygon2D;

public:
	PolygonSplitter() = default;
	~PolygonSplitter() = default;

	void clear();
	void setVerbose(bool v) { verbose_ = v; }

	int addVertex(double x, double y);
	int addEdge(double x1, double y1, double x2, double y2);
	int addEdge(int v1, int v2);

	int  findNextEdge(int edge, bool ignore_valid = true, bool only_convex = true, bool ignore_inpoly = false);
	void flipEdge(int edge);

	void detectConcavity();
	bool detectUnclosed();

	bool tracePolyOutline(int edge_start);
	bool testTracePolyOutline(int edge_start);

	bool splitFromEdge(int splitter_edge);
	bool buildSubPoly(int edge_start, Polygon2D::SubPoly* poly);
	bool doSplitting(Polygon2D* poly);

	// Testing
	void openSector(MapSector* sector);
	void testRender();

private:
	// Structs
	struct Edge
	{
		int  v1, v2;
		bool ok;
		bool done;
		bool inpoly;
		int  sister;
	};
	struct Vertex
	{
		double      x, y;
		vector<int> edges_in;
		vector<int> edges_out;
		bool        ok       = true;
		double      distance = 0.;
		Vertex(double x = 0., double y = 0.) : x{ x }, y{ y } {}

		operator fpoint2_t() const { return { x, y }; }
	};
	struct Outline
	{
		vector<int> edges;
		BBox      bbox;
		bool        clockwise;
		bool        convex;
	};

	// Splitter data
	vector<Vertex>  vertices_;
	vector<Edge>    edges_;
	vector<int>     concave_edges_;
	vector<Outline> polygon_outlines_;
	int             split_edges_start_ = 0;
	bool            verbose_           = false;
	double          last_angle_        = 0.;
};
