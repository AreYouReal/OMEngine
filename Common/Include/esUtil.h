#ifndef ESUTIL_H
#define ESUTIL_H

///
//  Includes
//
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif
#ifdef __cplusplus

extern "C" {
#endif


///
//  Macros
//
#ifdef WIN32
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl
#else
#define ESUTIL_API
#define ESCALLBACK
#endif

#define ES_WINDOW_RGB           0   /// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_ALPHA         1   /// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_DEPTH         2   /// esCreateWindow flag - depth buffer
#define ES_WINDOW_STENCIL       4   /// esCreateWindow flag - stencil buffer
#define ES_WINDOW_MULTISAMPLE   8   /// esCreateWindow flat - multi-sample buffer

///
// Types
//
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef struct ESContext ESContext;

struct ESContext{
    void       *platformData;    /// Put platform specific data here
    void       *userData;        /// Put your user data here...
    GLint       width;           /// Window width
    GLint       height;          /// Window height

#ifndef __APPLE__
   EGLNativeDisplayType eglNativeDisplay;   /// Display handle
   EGLNativeWindowType  eglNativeWindow;    /// Window handle
   EGLDisplay  eglDisplay;                  /// EGL display
   EGLContext  eglContext;                  /// EGL context
   EGLSurface  eglSurface;                  /// EGL surface
#endif

   /// Callbacks
   void ( ESCALLBACK *drawFunc ) ( ESContext * );
   void ( ESCALLBACK *shutdownFunc ) ( ESContext * );
   void ( ESCALLBACK *keyFunc ) ( ESContext *, unsigned char, int, int );
   void ( ESCALLBACK *updateFunc ) ( ESContext *, float deltaTime );
};


///
//  Public Functions
//

GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags );

void ESUTIL_API esRegisterDrawFunc ( ESContext *esContext, void ( ESCALLBACK *drawFunc ) ( ESContext * ) );

void ESUTIL_API esRegisterShutdownFunc ( ESContext *esContext, void ( ESCALLBACK *shutdownFunc ) ( ESContext * ) );

void ESUTIL_API esRegisterUpdateFunc ( ESContext *esContext, void ( ESCALLBACK *updateFunc ) ( ESContext *, float ) );

void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext,
                                    void ( ESCALLBACK *drawFunc ) ( ESContext *, unsigned char, int, int ) );

void ESUTIL_API esLogMessage ( const char *formatStr, ... );

//
/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
/// \param numSlices The number of slices in the sphere
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals,
                             GLfloat **texCoords, GLuint **indices );

//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLES
//
int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals,
                           GLfloat **texCoords, GLuint **indices );

//
/// \brief Generates a square grid consisting of triangles.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list as TRIANGLES.
/// \param size create a grid of size by size (number of triangles = (size-1)*(size-1)*2)
/// \param vertices If not NULL, will contain array of float3 positions
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLES
//
int ESUTIL_API esGenSquareGrid ( int size, GLfloat **vertices, GLuint **indices );

//
/// \brief Loads a 8-bit, 24-bit or 32-bit TGA image from a file
/// \param ioContext Context related to IO facility on the platform
/// \param fileName Name of the file on disk
/// \param width Width of loaded image in pixels
/// \param height Height of loaded image in pixels
///  \return Pointer to loaded image.  NULL on failure.
//
char *ESUTIL_API esLoadTGA ( void *ioContext, const char *fileName, int *width, int *height );


#ifdef __cplusplus
}
#endif

#endif // ESUTIL_H
