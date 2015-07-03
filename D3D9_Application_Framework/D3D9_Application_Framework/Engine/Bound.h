//Represent a ray
//Parametrization as follows: R(t) = P + t * D
//	Where: -P is the origin of the ray
//		   -D is the direction of the ray, must be unitary
//		   -f is the scalar parametrization [0,tmax]
class Ray
{
	public:

		D3DXVECTOR3 P;
		D3DXVECTOR3 D;

		float tmax;



};

class CollisionResult
{
	D3DXVECTOR3 Point;
	//parametrization value
	//Object to wich we collide
	
};



class Bound
{
public:
	//For axis oriented box
	//Tells if the ray that is passed collides with this object
	//But gives no information about where
	//
	//return:		if true -> collides else not collides
	//
	//Note: Faster than the version that gives collision info
	bool TestIntersection(IN Ray ray)
	{
		//Posible 6 separating axis:
		D3DXVECTOR3& W1 = LocalX;
		D3DXVECTOR3& W2 = LocalY;
		D3DXVECTOR3& W3 = LocalZ;
		D3DXVECTOR3  W4; D3DXVec3Cross(Ray.D,LocalX,&W4);
		D3DXVECTOR3  W5; D3DXVec3Cross(Ray.D,LocalY,&W5);
		D3DXVECTOR3  W6; D3DXVec3Cross(Ray.D,LocalZ,&W6);

		//Distance from Box center to Ray begin
		D3DXVECTOR3 D = Ray.P - Center;

		//W1:
		float WLength = 0;
		D3DXVec3Length(W1, WLength);
		float ProjBoxRadius = 0;
		ProjBoxRadius += e1 * abs( (LocalX * W) / WLength );
		ProjBoxRadius += e2 * abs( (LocalY * W) / WLength );
		ProjBoxRadius += e2 * abs( (LocalZ * W) / WLength );
		
		float ProjDRadius = abs( (D * W) / WLength );


		// The Global question for each axis:
		//IF	Radius of Box < Radius of Distance	AND	 Ray.Direction is oposite to the box

		if( (abs(LocalX*D) > e0) && ((W*ray.D)*(W*D) >= 0.0f) )
			return;
		if( (abs(LocalY*D) > e1) && ((W*ray.D)*(W*D) >= 0.0f) )
			return;
		if( (abs(LocalZ*D) > e2) && ((W*ray.D)*(W*D) >= 0.0f) )
			return;

		D3DXVECTOR3  aux;  D3DXVec3Cross(Ray.D,D,&aux);
		float  aux2 = abs( D3DXVec3Dot(ray.D,LocalX,&aux2) );
		float  aux3 = abs( D3DXVec3Dot(ray.D,LocalY,&aux3) );
		float  aux4 = abs( D3DXVec3Dot(ray.D,LocalZ,&aux4) );
		if( abs(LocalX*aux) > (e1*aux4+e2*aux3) )
			return;
		if( abs(LocalY*aux) > (e0*aux4+e2*aux2) )
			return;
		if( abs(LocalZ*aux) > (e0*aux3+e1*aux2) )
			return;

		return;
	}

	//For axis oriented box
	//Uses Liang-Barsky
	//Tells if the ray that is passed collides with this object
	//But gives information about where happen the collision
	//
	//return:		if true -> collides else not collides
	//
	//Note: Slower than the version that not't give collision info
	bool TestIntersection(IN Ray ray, OUT CollisionResult& result);

	D3DXVECTOR3 Center;

};