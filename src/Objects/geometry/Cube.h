/*
Cube

Depends on: Polygon.h

Description: A 3d cube that forms the basis for amy cubes  we may want to construct in the future


origin: the point of origin for this particular polygon
vaComponentCount: the number of vertex components in this Cube 
vaByteSize: the number Bytes required to contain the vertices of this cube
*/

#include "Polygon.h"

class VertexComponent;

#include "../../Opengl_a/VertexController.h"

class Cube : public Polygon{

	public:
		Cube(glm::vec3 origin_a);
		virtual void transform(glm::mat4 transmat);
		void translate_fromOrigin();
		virtual float* getVertexArray();
		virtual int getVAVertexCount();
		virtual int getVAByteSize();
		virtual int getVAFloatCount();
		virtual int getVertexByteSize();
		virtual void setVertexController(bool position, bool texture, bool color, bool normal);
		virtual void setBoolean(bool position, bool texture, bool color, bool normal);
		virtual Vertex getSampleVertex();
		virtual std::map<std::string, glm::vec3> getMinMax();
		virtual void print();
		

		glm::vec3 origin;
		int vaComponentCount;
		int vaByteSize;

		void setupVC();

		VertexController* vc;

		
};
