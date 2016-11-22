
class Object3d
{



public:
	enum Primitves
	{
		Quad, Cube, Sphere, Cylinder
	};

	Object3d();
	Object3d(Primitves primitve);
	~Object3d();

	float* vertices;
};
