#ifndef _OGLSHADER_HPP_INCLUDED_
#define _OGLSHADER_HPP_INCLUDED_

#include <vector>
#include <map>

#include "ogldriver.hpp"
#include "shadertypes.hpp"

#include "core/fileio/file.hpp"

#include <string>
using std::getline;
using std::string;

using std::map;

namespace shader
{

   enum eShaderType
   {
      SHADER_VERTEX = GL_VERTEX_SHADER,
      SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
      SHADER_GEOMETRY = GL_GEOMETRY_SHADER,
      SHADER_TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
      SHADER_TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER
   };

   class GLSLShader
   {
   private:
      GLuint m_program;
      int32 m_totalShaders;
      //eShaderType m_type;
      map<string, GLuint> m_attributeMap;
      map<string, GLuint> m_uniformLocationMap;
      enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER }; //tmp
      GLuint m_shaders[3];//0->vertexshader, 1->fragmentshader, 2->geometryshader //tmp
   public:
      GLSLShader();
      ~GLSLShader();
      void Load(GLenum type, const string &filename);
      GLuint operator[](const string &attribute);
      GLuint operator()(const string &uniform);

      void AddUniform(const string &uniform);
      void AddUniformData(const char *variableName, const void *_array, eVectorType type, int32 numElementsToModify);
      void AddUniformData(const char *variableName, const void *_array, eMatrixType type, int32 numMatrices, bool transposed = false);
      void AddUniformSampler(const char* variableName, eOpaqueType sampler);

      void Use();
      void Unuse();
      void AddAttribute(const string &);
      void DeleteProgram();
      void CreateAndLink();
      //void GetCompilationStatus(string &outStatus) const;
   };

   struct ProgramID // ??
   {
      uint32 type;
      int32 location;
      int32 arraySize;
      int32 isInt;
      int32 order; ///< order that GLSL var is defined. For Sampler variables, its also the Texture Unit
   };

   //Linked and Compiled program to run on the GPU, composition of one or several source 
   class ShaderExecutable
   {
   private:
      uint32 handle;
      //std::vector<uint32> uniformHandles;
      //int32 uniformHandle; // midlertidig forenkling med 1 uniform dørhåndtak
      map<const char*, int32> handleMap;

      //can have varying number of source files for each type of shader: vertex, geometry and fragment
      //vector<Shader> sources;
   public:
      ShaderExecutable();
      ~ShaderExecutable();
      bool Create();
      void AddUniformLocation(const char *name);
      void AddUniformData(const char *variableName, const void *_array, eVectorType type, int32 numElementsToModify);
      void AddUniformData(const char *variableName, const void *_array, eMatrixType type, int32 numMatrices, bool transposed = false);
      void AddUniformSampler(const char* variableName, eOpaqueType sampler);
      void Attach(uint32 shaderHandle) const;
      void Detach(uint32 shaderHandle) const;
      void Link() const;

      void Run();
      uint32 GetHandle() const;
   };

} // namespace shader

#endif