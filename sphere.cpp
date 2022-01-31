#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    Hit ans;
    ans.object = nullptr;
    ans.dist = 0;
    vec3 e_c = ray.endpoint-center;
    double a = dot(ray.direction, ray.direction);
    double b = dot(ray.direction, e_c);
    double c = e_c.magnitude_squared()-radius*radius;
    double delta = b*b-a*c;

    if(delta>0){
        double x1 = (-b+sqrt(delta))/(a);
        double x2 = (-b-sqrt(delta))/(a);
        if(x1>=small_t && x2>=small_t ){
            ans.object = this;
            ans.dist = std::min(x1,x2);
        }else if(x1<small_t && x2>=small_t ){
            ans.object = this;
            ans.dist = x2;
        }else if(x1>=small_t && x2<small_t ){
            ans.object = this;
            ans.dist = x1;
        }
    }
    return ans;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal = (point-center).normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    box.Make_Empty();
    for(int i=0;i<3;i++){
        box.lo[i]=center[i]-radius;
        box.hi[i]=center[i]+radius;
    }
    return box;
}
