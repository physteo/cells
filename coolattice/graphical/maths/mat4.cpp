
#include "mat4.h"

namespace sparky { namespace maths {


    mat4::mat4()
    {
        for(int i = 0; i < 4 * 4; i++)
            elements[i] = 0.0f;
    }

    mat4::mat4(float diagonal)
    {
        for(int i = 0; i < 4 * 4; i++)
            elements[i] = 0.0f;

        elements[0 + 0 * 4] = diagonal;
        elements[1 + 1 * 4] = diagonal;
        elements[2 + 2 * 4] = diagonal;
        elements[3 + 3 * 4] = diagonal;
    }

    mat4 mat4::identity()
    {
        return mat4(1.0f);
    }


    mat4& mat4::multiply(const mat4& other)
    {
        mat4 result;
        for(int y = 0; y < 4; y++)
        {
            for(int x = 0; x < 4; x++)
            {
                float sum = 0.0f;
                for(int e = 0; e < 4; e++)
                {
                    sum += elements[x + e * 4] * other.elements[e + y * 4];
                }
                elements[x + y * 4] = sum;
            }
        }
        return *this;
    }


	// mod(matteo)
	mat4& mat4::sum(const mat4& other)
	{
		mat4 result;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				elements[x + y * 4] += other.elements[x + y * 4];
			}
		}
		return *this;
	}

	mat4 operator+(mat4 left, const mat4& right)
	{
		return left.sum(right);
	}
	mat4& mat4::operator+=(const mat4& other)
	{
		return sum(other);
	}

    mat4 operator*(mat4 left, const mat4& right)
    {
        return left.multiply(right);
    }

    mat4& mat4::operator*=(const mat4& other)
    {
        return multiply(other);
    }


    mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        mat4 result(1.0f);
        result.elements[0 + 0 * 4] = 2.0f/(right - left);
        result.elements[1 + 1 * 4] = 2.0f/(top - bottom);
        result.elements[2 + 2 * 4] = 2.0f/(near - far);
        result.elements[0 + 3 * 4] = (left + right) / (left - right);
        result.elements[1 + 3 * 4] = (bottom + top) / (bottom - top);
        result.elements[2 + 3 * 4] = (far + near)   / (far - near);

        return result;
    }

    mat4 mat4::perspective(float fov, float aspectRatio, float near, float far)
    {
        mat4 result(1.0f);

        float q = 1.0f / tan(toRadiants(0.5f * fov));
        float a =   q / aspectRatio;
        float b =   (near + far) / (near - far);
        float c =   (2.0f * near * far) / (near - far);

        result.elements[0 + 0 * 4] = a;
        result.elements[1 + 1 * 4] = q;
        result.elements[2 + 2 * 4] = b;
        result.elements[3 + 2 * 4] = -1.0f;
        result.elements[2 + 3 * 4] = c;

        return result;
    }

	// mod(matteo)
	mat4 mat4::view(const vec3& cameraPos, float cameraPitch, float cameraYaw, float cameraRoll)
	{
		//TODO: ROLL NOT IMPLEMENTED
		
		// Set identity
		mat4 result(1.0f);
		return	translation(vec3{ -cameraPos.x,-cameraPos.y,-cameraPos.z }) * 
				rotation(toRadiants(cameraYaw), vec3{ 0,1,0 }) * 
				rotation(toRadiants(cameraPitch), vec3{ 1,0,0 });
	}

    mat4 mat4::translation(const vec3& translation)
    {
        mat4 result(1.0f);
        result.elements[0 + 3 * 4] = translation.x;
        result.elements[1 + 3 * 4] = translation.y;
        result.elements[2 + 3 * 4] = translation.z;

        return result;
    }

     mat4 mat4::scale(const vec3& scale)
     {
        mat4 result(1.0f);
        result.elements[0 + 0 * 4] = scale.x;
        result.elements[1 + 1 * 4] = scale.y;
        result.elements[2 + 2 * 4] = scale.z;
        return result;
     }

     mat4 mat4::rotation(float angle, const vec3& axis)
     {
//		 float radiants = toRadiants(angle);
//		 float x = axis.x;
//		 float y = axis.y;
//		 float z = axis.z;
//
//		 float norm = sqrt(x*x + y*y + z*z);
//		 x /= norm;
//		 y /= norm;
//		 z /= norm;
//
//		 mat4 firstTerm = cos(radiants)*mat4(1.0);
//		 firstTerm.elements[3 + 3 * 4] = 0.0;
//
//		 mat4 secondTerm = mat4(0.0);
//		 secondTerm.elements[0 + 0 * 4] = x * x;
//		 secondTerm.elements[1 + 0 * 4] = x * y;
//		 secondTerm.elements[2 + 0 * 4] = x * z;
//
//		 secondTerm.elements[0 + 1 * 4] = x * y;
//		 secondTerm.elements[1 + 1 * 4] = y * y;
//		 secondTerm.elements[2 + 1 * 4] = y * z;
//
//		 secondTerm.elements[0 + 2 * 4] = x * z;
//		 secondTerm.elements[1 + 2 * 4] = y * z;
//		 secondTerm.elements[2 + 2 * 4] = z * z;
//
//		 secondTerm = (1.0 - cos(radiants)) * secondTerm;
//		 
//
//
//		 mat4 thirdTerm = mat4(0.0);
//		 thirdTerm.elements[0 + 0 * 4]  =  0;
//		 thirdTerm.elements[1 + 0 * 4]  = -z;
//		 thirdTerm.elements[2 + 0 * 4]  =  y;
//
//		 thirdTerm.elements[0 + 1 * 4]  =  z;
//		 thirdTerm.elements[1 + 1 * 4]  =  0;
//		 thirdTerm.elements[2 + 1 * 4]  = -x;
//
//		 thirdTerm.elements[0 + 2 * 4]  = -y;
//		 thirdTerm.elements[1 + 2 * 4]  =  x;
//		 thirdTerm.elements[2 + 2 * 4]  =  0;
//
//		 thirdTerm = thirdTerm * sin(radiants);
//
//		 auto sum = firstTerm + secondTerm + thirdTerm;
//		 sum.elements[3 + 3 * 4] = 1.0;
//		 return sum;
		 //return glm::transpose(firstTerm + secondTerm + thirdTerm);

        mat4 result(1.0f);

        float r = toRadiants(angle);
        float c = cos(r);
        float s = sin(r);
        float omc = 1.0f - c;

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

		result.elements[0 + 0 * 4] = x * x * omc + c;
		result.elements[0 + 1 * 4] = y * x * omc + z * s;
		result.elements[0 + 2 * 4] = x * z * omc - y * s;

		result.elements[1 + 0 * 4] = x * y * omc - z * s;
		result.elements[1 + 1 * 4] = y * y * omc + c;
		result.elements[1 + 2 * 4] = y * z * omc + x * s;

		result.elements[2 + 0 * 4] = x * z * omc + y * s;
		result.elements[2 + 1 * 4] = y * z * omc - x * s;
		result.elements[2 + 2 * 4] = z * z * omc + c;

        return result;
     }



	 //float r = 2. * pi * (degrees) / (360.);
	 //float x = axis.x;
	 //float y = axis.y;
	 //float z = axis.z;
	 //
	 //float norm = sqrt(x*x + y*y + z*z);
	 //x /= norm;
	 //y /= norm;
	 //z /= norm;
	 //
	 //mat3 firstTerm = cos(r) * mat3(1.0);
	 //mat3 secondTerm = (1.0 - cos(r)) * mat3(x*x, x*y, x*z,
	//											x*y, y*y, y*z,
	//											x*z, y*z, z*z);
	 //mat3 thirdTerm = sin(r) * mat3(0, -z, y,
	//	 z, 0, -x,
	//	 -y, x, 0);
	 //return glm::transpose(firstTerm + secondTerm + thirdTerm);










    std::ostream& operator<<(std::ostream& stream, const mat4& matrix)
    {
        stream << "\t" << matrix.elements[0] << matrix.elements[4] << matrix.elements[8] << matrix.elements[12] << std::endl;
        stream << "\t" << matrix.elements[1] << matrix.elements[5] << matrix.elements[9] << matrix.elements[13] << std::endl;
        stream << "\t" << matrix.elements[2] << matrix.elements[6] << matrix.elements[10] << matrix.elements[14] << std::endl;
        stream << "\t" << matrix.elements[3] << matrix.elements[7] << matrix.elements[11] << matrix.elements[15] << std::endl;
        return stream;
    }


}

}
