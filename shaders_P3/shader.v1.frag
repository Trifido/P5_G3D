#version 330 core
#extension GL_ARB_shading_language_420pack : enable //Habilita una extension de OpenGL 4.2 para usar el layout (binding)

#define MAX_SIZE 8u

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;
in vec3 tangent;

//Texturas del objeto

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

//Intensidad de luz ambiental
uniform vec3 Ia;

//Arrays de coordenadas de luces
uniform vec3 PosPoint[MAX_SIZE];
uniform vec3 PosSpot[MAX_SIZE];
uniform vec3 PosDirec[MAX_SIZE];

//Arrays de intensidades de luces
uniform vec3 intPoint[MAX_SIZE];
uniform vec3 intSpot[MAX_SIZE];
uniform vec3 intDirec[MAX_SIZE];

//N�mero m�ximo de luces
uniform int numPoint;
uniform int numSpot;
uniform int numDirec;

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
vec3 N;

float n;

vec3 shade(int i);
vec3 spotShade(int i);
vec3 direcShade(int i);
vec3 CalcBumpedNormal();

void main()
{
	Ka = texture(colorTex, texCoord).rgb;
	Kd = Ka;
	Ke = texture(emiTex, texCoord).rgb;
	Ks = texture(specularTex, texCoord).xyz;
	
	n = 200.0;
	
	N = normalize (norm);
	N = CalcBumpedNormal();

	

	outColor = vec4(0);

	for(int i = 0; i < numPoint; i++)
	{
		outColor += vec4(shade(i), 1.0); 
	}

	for(int i = 0; i < numSpot; i++)
	{
		outColor += vec4(spotShade(i), 1.0);   
	}

	for(int i = 0; i < numDirec; i++)
	{
		outColor += vec4(direcShade(i), 1.0);   
	}
}

vec3 shade(int i)
{
	vec3 color = Ia * Ka;

	//Diffuse 
	vec3 L = normalize (PosPoint[i] - pos);
	vec3 diffuse = intPoint[i] * Kd * dot (L,N);
	color += clamp(diffuse, 0.0, 1.0);
	
	//Specular
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.01);

	vec3 specular = intPoint[i] * Ks * pow(factor,n);
	color += clamp(specular, 0.0, 1.0);

	//Color-Textura emisivo
	color += Ke;
	
	return color;
}

vec3 spotShade(int i)
{
	vec3 Ip;
	vec3 color = Ia * Ka;

	vec3 D = normalize(PosSpot[i]);
	vec3 L = normalize(vec3(PosSpot[i] - pos));

	vec3 diff = intSpot[i] * Kd * dot(N, L);	

	float radiAng = 15.0 * 3.1416 / 180.0;
	float cosAng = cos(radiAng);
	float escPro = dot(D, L);

	if( escPro > cosAng){

		vec3 V = normalize(-pos);
		vec3 R = reflect (-L, N); //Se usa -L porque L es la direcci�n contraria del rayo.
		float Sfactor = max (dot(R, V), 0.0001);
		Sfactor = pow (Sfactor,n);
		vec3 spec = intSpot[i] * Ks * Sfactor;

		Ip = pow( (escPro - cosAng) / (1.0 - cosAng) , n) * (diff + spec);

	}
	else{
		Ip = vec3(0.0);
		color += diff;
	}

	color += clamp(Ip, 0.0, 1.0);
	color += Ke;

	return color;
}

vec3 direcShade(int i)
{
	vec3 color = Ia * Ka;

	//Diffuse 
	vec3 L = normalize (PosDirec[i]);
	vec3 diffuse = intDirec[i] * Kd * dot (L,N);
	color += clamp(diffuse, 0.0, 1.0);
	
	//Specular
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.0001);

	vec3 specular = intDirec[i] * Ks * pow(factor,n);
	color += clamp(specular, 0.0, 1.0);

	//Color-Textura emisivo
	color += Ke;
	
	return color;
}

vec3 CalcBumpedNormal()
{
    vec3 Tangent = normalize(tangent);
    Tangent = normalize(Tangent - dot(Tangent, N) * N);
    vec3 Bitangent = cross(Tangent, N);
    vec3 BumpMapNormal = texture(normalTex, texCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, N);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}