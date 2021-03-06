#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <Windows.h>
#include <map>
#include <cmath>

using namespace std;

bool alive = true;

#define BUFFER_VERTICES  0
#define BUFFER_INDICES   1
#define BUFFER_MATRICES  2
#define BUFFER_VERTICES2 3
#define BUFFER_INDICES2  4

#define TEXTURE_DIFFUSE  0
#define TEXTURE_POSITION 1
#define TEXTURE_NORMAL   2

namespace gl
{
    HMODULE opengl = NULL;
    multimap<void *, const char *> *imports = NULL;

    HGLRC (_stdcall *wglCreateContext)(HDC);
    BOOL (_stdcall *wglMakeCurrent)(HDC, HGLRC);
    BOOL (_stdcall *wglDeleteContext)(HGLRC);
    PROC (_stdcall *wglGetProcAddress)(LPCSTR) = NULL;
    HGLRC (_stdcall *wglCreateContextAttribs)(HDC, HGLRC, const int *);
    BOOL (_stdcall *wglChoosePixelFormat)(HDC, const int *, const FLOAT *, UINT, int *, UINT *);

#define WGL_CONTEXT_DEBUG_BIT                   0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT      0x0002
#define WGL_CONTEXT_MAJOR_VERSION               0x2091
#define WGL_CONTEXT_MINOR_VERSION               0x2092
#define WGL_CONTEXT_LAYER_PLANE                 0x2093
#define WGL_CONTEXT_FLAGS                       0x2094
#define WGL_CONTEXT_PROFILE_MASK                0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT            0x0001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT   0x0002

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

    typedef unsigned int GLenum;
    typedef bool GLboolean;
    typedef unsigned int GLbitfield;
    typedef signed char GLbyte;
    typedef signed short GLshort;
    typedef signed int GLint;
    typedef signed int GLsizei;
    typedef unsigned char GLubyte;
    typedef unsigned short GLushort;
    typedef unsigned int GLuint;
    typedef unsigned short GLhalf;
    typedef float GLfloat;
    typedef float GLclampf;
    typedef double GLdouble;
    typedef double GLclampd;
    typedef void GLvoid;
    typedef char GLchar;
#if defined(_M_X64) || defined(__amd64__)
    typedef signed long long GLintptr;
    typedef signed long long GLsizeiptr;
#elif defined(_M_IX86) || defined(__i386__)
    typedef signed int GLintptr;
    typedef signed int GLsizeiptr;
#else
#error No define specifying architecture
#endif

#define GL_FALSE 0
#define GL_TRUE  1
#define GL_NONE  0

    void *register_import(void *pointer, const char *name)
    {
        if (imports == NULL)
        {
            imports = new multimap<void *, const char *>;
        }

        if (imports == NULL)
        {
            throw runtime_error("OpenGL binding");
        }

        imports->insert(multimap<void *, const char *>::value_type(pointer, name));

        return NULL;
    }

#define BIND(result, name, ...) \
    typedef result (APIENTRYP BIND_GL_##name)(__VA_ARGS__); \
    GLAPI BIND_GL_##name name = reinterpret_cast<BIND_GL_##name>(register_import(&name, #name));

BIND(GLvoid, glEnable, GLenum);
BIND(GLvoid, glDisable, GLenum);
#define GL_TEXTURE_2D 0x0DE1
#define GL_DEPTH_TEST 0x0B71

BIND(GLubyte *, glGetString, GLenum);
#define GL_RENDERER                 0x1F01
#define GL_VENDOR                   0x1F00
#define GL_VERSION                  0x1F02
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
BIND(GLvoid, glViewport, GLint, GLint, GLsizei, GLsizei);

BIND(GLvoid, glClearColor, GLclampf, GLclampf, GLclampf, GLclampf);
BIND(GLvoid, glClear, GLbitfield);
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_DEPTH_BUFFER_BIT 0x0100

BIND(GLvoid, glGenBuffers, GLsizei, GLuint *);
BIND(GLvoid, glDeleteBuffers, GLsizei, const GLuint *);
BIND(GLvoid, glBindBuffer, GLenum, GLuint);
#define GL_ARRAY_BUFFER         0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
BIND(GLvoid, glBufferData, GLenum, GLsizeiptr, const GLvoid *, GLenum);
#define GL_STATIC_DRAW 0x88E4

BIND(GLvoid, glGenVertexArrays, GLsizei, GLuint *);
BIND(GLvoid, glDeleteVertexArrays, GLsizei, const GLuint *);
BIND(GLvoid, glBindVertexArray, GLuint);
BIND(GLvoid, glEnableVertexAttribArray, GLuint);
BIND(GLvoid, glDisableVertexAttribArray, GLuint);

BIND(GLvoid, glVertexAttribPointer, GLuint, GLsizei, GLenum, GLboolean, GLsizei, const GLvoid *);
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT  0x1405
#define GL_FLOAT         0x1406
BIND(GLvoid, glVertexAttribDivisor, GLuint, GLuint);

BIND(GLuint, glCreateShader, GLuint);
#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
BIND(GLvoid, glShaderSource, GLuint, GLsizei, const GLchar **, const GLint *);
BIND(GLvoid, glCompileShader, GLuint);
BIND(GLvoid, glDeleteShader, GLuint);

BIND(GLuint, glCreateProgram, void);
BIND(GLvoid, glAttachShader, GLuint, GLuint);
BIND(GLvoid, glDetachShader, GLuint, GLuint);
BIND(GLvoid, glLinkProgram, GLuint);
BIND(GLvoid, glUseProgram, GLuint);
BIND(GLvoid, glDeleteProgram, GLuint);

BIND(GLint, glGetAttribLocation, GLuint, const GLchar *);
BIND(GLint, glGetUniformLocation, GLuint, const GLchar *);
BIND(GLvoid, glBindFragDataLocation, GLuint, GLuint, const GLchar *);

BIND(GLvoid, glUniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat *);
BIND(GLvoid, glUniform1f, GLint, GLfloat);
BIND(GLvoid, glUniform1i, GLint, GLint);

BIND(GLvoid, glDrawElements, GLenum, GLsizei, GLenum, const GLvoid *);
#define GL_TRIANGLES 0x0004
BIND(GLvoid, glDrawElementsInstanced, GLenum, GLsizei, GLenum, const GLvoid *, GLsizei);

BIND(GLvoid, glGenFramebuffers, GLsizei, GLuint *);
BIND(GLvoid, glDeleteFramebuffers, GLsizei, GLuint *);
BIND(GLvoid, glBindFramebuffer, GLenum, GLuint);
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_FRAMEBUFFER      0x8D40
BIND(GLvoid, glFramebufferRenderbuffer, GLenum, GLenum, GLenum, GLuint);
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_DEPTH_ATTACHMENT  0x8D00
BIND(GLvoid, glFramebufferTexture2D, GLenum, GLenum, GLenum, GLuint, GLint);
BIND(GLvoid, glDrawBuffers, GLsizei, const GLenum *);
BIND(GLvoid, glDrawBuffer, GLenum);
#define GL_BACK 0x0405

BIND(GLvoid, glGenRenderbuffers, GLsizei, GLuint *);
BIND(GLvoid, glDeleteRenderbuffers, GLsizei, GLuint *);
BIND(GLvoid, glBindRenderbuffer, GLenum, GLuint);
#define GL_RENDERBUFFER 0x8D41
BIND(GLvoid, glRenderbufferStorage, GLenum, GLenum, GLsizei, GLsizei);
#define GL_DEPTH_COMPONENT24 0x81A6

BIND(GLvoid, glGenTextures, GLsizei, GLuint *);
BIND(GLvoid, glDeleteTextures, GLsizei, GLuint *);
BIND(GLvoid, glBindTexture, GLenum, GLuint);
#define GL_TEXTURE_2D 0x0DE1
BIND(GLvoid, glTexImage2D, GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, GLvoid *data);
#define GL_RGB     0x1907
#define GL_RGBA    0x1908
#define GL_RGBA32F 0x8814
#define GL_RGBA16F 0x805B
#define GL_RGB32F  0x8815
#define GL_RGB16F  0x881B
BIND(GLvoid, glTexParameteri, GLenum, GLenum, GLint);
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S     0x2802
#define GL_TEXTURE_WRAP_T     0x2803
#define GL_NEAREST            0x2600
#define GL_LINEAR             0x2601
#define GL_REPEAT             0x2901
#define GL_CLAMP_TO_EDGE      0x812F
BIND(GLvoid, glActiveTexture, GLenum);
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2

    bool init_context(bool has_context = true)
    {
        PROC GetProcAddressW = GetProcAddress(opengl, "wglGetProcAddress");
        PROC CreateContext = GetProcAddress(opengl, "wglCreateContext");
        PROC MakeCurrent = GetProcAddress(opengl, "wglMakeCurrent");
        PROC DeleteContext = GetProcAddress(opengl, "wglDeleteContext");

        if (!GetProcAddressW || !CreateContext || !MakeCurrent || !DeleteContext)
        {
            return false;
        }

        wglGetProcAddress = reinterpret_cast<PROC (_stdcall *)(LPCSTR)>(GetProcAddressW);
        wglCreateContext = reinterpret_cast<HGLRC (_stdcall *)(HDC)>(CreateContext);
        wglMakeCurrent = reinterpret_cast<BOOL (_stdcall *)(HDC, HGLRC)>(MakeCurrent);
        wglDeleteContext = reinterpret_cast<BOOL (_stdcall *)(HGLRC)>(DeleteContext);

        if (!has_context)
        {
            return true;
        }

        wglCreateContextAttribs = reinterpret_cast<HGLRC (_stdcall *)(HDC, HGLRC, const int *)>(wglGetProcAddress("wglCreateContextAttribsARB"));
        wglChoosePixelFormat = reinterpret_cast<BOOL (_stdcall *)(HDC, const int *, const FLOAT *, UINT, int *, UINT *)>(wglGetProcAddress("wglChoosePixelFormatARB"));

        if (!wglCreateContextAttribs || !wglChoosePixelFormat)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool init()
    {
        if (opengl == NULL)
        {
            opengl = LoadLibraryEx("OpenGL32.dll", NULL, 0);
        }

        if (opengl == NULL)
        {
            return false;
        }

        if(init_context(false) == false)
        {
            FreeLibrary(opengl);

            return false;
        }

        return true;
    }

    bool import()
    {
        if (imports != NULL)
        {
            for (multimap<void *, const char *>::iterator i = imports->begin(); i != imports->end(); ++i)
            {
                PROC *entrypoint = reinterpret_cast<PROC *>(i->first);

                if (*entrypoint == NULL)
                {
                    *entrypoint = GetProcAddress(opengl, i->second);

                    if (*entrypoint == NULL)
                    {
                        *entrypoint = wglGetProcAddress(i->second);
                    }

                    if (*entrypoint == NULL)
                    {
                        cerr << "Could not find entrypoint for " << i->second << endl;

                    }
                }
            }
        }

        return true;
    }

    void cleanup()
    {
        FreeLibrary(opengl);
    }
}

using namespace gl;

struct shader_stage1
{
    static const GLchar *source_vertex;
    static const GLchar *source_geometry;
    static const GLchar *source_fragment;

    GLuint shader_vertex;
    GLuint shader_geometry;
    GLuint shader_fragment;

    GLuint program;

    GLint att_vertex;
    GLint att_color;
    GLint att_normal;
    GLint att_matrix;

    GLint uni_perspective;
    GLint uni_counter;

    shader_stage1();
    ~shader_stage1();
};

struct shader_stage2
{
    static const GLchar *source_vertex;
    static const GLchar *source_fragment;

    GLuint shader_vertex;
    GLuint shader_fragment;

    GLuint program;

    GLuint format;

    GLuint buffer_vertices;
    GLuint buffer_indices;

    GLint att_vertex;

    GLint uni_perspective;
    GLint uni_color;
    GLint uni_position;
    GLint uni_normal;

    shader_stage2();
    ~shader_stage2();
};

struct galaxy
{
    GLuint format;

    GLuint buffer_vertices;
    GLuint buffer_indices;

    shader_stage1 *shader;

    galaxy(shader_stage1 *shader);
    ~galaxy();
};

LRESULT CALLBACK application_loop(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_DESTROY:
        alive = false;
        break;

    case WM_PAINT:
        PAINTSTRUCT ps;

        BeginPaint(window, &ps);
        EndPaint(window, &ps);
        break;

    case WM_SIZE:
        break;

    default:
        return DefWindowProc(window, message, wparam, lparam);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int exit = EXIT_SUCCESS;

    /* Window */
    HINSTANCE instance;
    ATOM atom;
    HWND window;
    unsigned long int style = WS_POPUP;
    unsigned long int exstyle = WS_EX_APPWINDOW;

    /* Context */
    HDC device;
    HGLRC context;

    /* Bindings */
    if (gl::init() == false)
    {
        cerr << "Could not bind OpenGL entry points" << endl;

        return EXIT_FAILURE;
    }

    /* Create window */
    instance = GetModuleHandle(NULL);

    if (instance == NULL)
    {
        cerr << "Could not retrieve module handle" << endl;

        return EXIT_FAILURE;
    }

    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
    wc.lpfnWndProc = application_loop;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = DLGWINDOWEXTRA;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = 0;
    wc.lpszClassName = "planets 3d window";

    atom = RegisterClassEx(&wc);

    RECT view;

    view.left = 0;
    view.top = 0;
    view.right = GetSystemMetrics(SM_CXSCREEN);
    view.bottom = GetSystemMetrics(SM_CYSCREEN);

    if (AdjustWindowRectEx(&view, style, FALSE, exstyle) == 0)
    {
        cerr << "Could not adjust window rectangle" << endl;
    }

    window = CreateWindowEx(exstyle, "planets 3d window", "OpenGL planets", style, view.left, view.top, view.right - view.left, view.bottom - view.top, NULL, NULL, instance, NULL);

    if (window == NULL)
    {
        cerr << "Could not create window" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    device = GetDC(window);

    if (device == 0)
    {
        cerr << "Could not get device context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    /* Create context */
    PIXELFORMATDESCRIPTOR pixel;

    ZeroMemory(&pixel, sizeof(PIXELFORMATDESCRIPTOR));

    pixel.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixel.nVersion = 1;
    pixel.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixel.iPixelType = PFD_TYPE_RGBA;
    pixel.cColorBits = 24;
    pixel.cDepthBits = 8;
    pixel.iLayerType = PFD_MAIN_PLANE;

    int format = ChoosePixelFormat(device, &pixel);

    if (format <= 0)
    {
        cerr << "Could not choose pixel format" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    if (SetPixelFormat(device, format, &pixel) == FALSE)
    {
        cerr << "Could not set pixel format" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    context = gl::wglCreateContext(device);

    if (context == 0)
    {
        cerr << "Could not create OpenGL context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    if (gl::wglMakeCurrent(device, context) == 0)
    {
        cerr << "Could not set current rendering context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_context;
    }

    if (gl::init_context() == false)
    {
        cerr << "Could not bind entrypoint for advanced context creation" << endl;

        goto skip_context;
    }

    const int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION, 3,
        WGL_CONTEXT_MINOR_VERSION, 3,
        0
    };

    HGLRC backup = context;

    context = wglCreateContextAttribs(device, 0, attributes);

    if (context == 0)
    {
        cerr << "Could not create OpenGL context" << endl;

        exit = EXIT_FAILURE;

        goto cleanup_window;
    }

    if (gl::wglMakeCurrent(device, context) == 0 || init_context() == false)
    {
        if (gl::wglMakeCurrent(device, backup) == 0)
        {
            cerr << "Could not set current rendering context" << endl;

            exit = EXIT_FAILURE;

            goto cleanup_context;
        }
        else
        {
            gl::wglDeleteContext(context);

            context = backup;
        }
    }
    else
    {
        gl::wglDeleteContext(backup);
    }

skip_context:

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    gl::import();

    /* Do stuff */
    MSG msg;

    cout << "Rendering using " << glGetString(GL_RENDERER) << " by " << glGetString(GL_VENDOR) << endl;
    cout << "OpenGL version is " << glGetString(GL_VERSION) << ", GLSL version is " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    GLfloat matrices[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
       -7.5f, 2.5f, -15.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -5.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        2.5f, 5.0f, -10.0f, 1.0f,
    };

    shader_stage1 *stage1 = new shader_stage1;
    shader_stage2 *stage2 = new shader_stage2;

    galaxy *milky_way = new galaxy(stage1);

    GLuint buffers[5];
    GLuint array, array2;

    glGenVertexArrays(1, &array);
    glBindVertexArray(milky_way->format);
    glGenBuffers(5, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[BUFFER_MATRICES]);
    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(GLfloat), matrices, GL_STATIC_DRAW);

    glBindVertexArray(milky_way->format);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[BUFFER_MATRICES]);
    glVertexAttribPointer(stage1->att_matrix, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(stage1->att_matrix + 1, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat), reinterpret_cast<GLfloat *>(NULL) + 4);
    glVertexAttribPointer(stage1->att_matrix + 2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat), reinterpret_cast<GLfloat *>(NULL) + 8);
    glVertexAttribPointer(stage1->att_matrix + 3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat), reinterpret_cast<GLfloat *>(NULL) + 12);
    glVertexAttribDivisor(stage1->att_matrix, 1);
    glVertexAttribDivisor(stage1->att_matrix + 1, 1);
    glVertexAttribDivisor(stage1->att_matrix + 2, 1);
    glVertexAttribDivisor(stage1->att_matrix + 3, 1);
    glEnableVertexAttribArray(stage1->att_matrix);
    glEnableVertexAttribArray(stage1->att_matrix + 1);
    glEnableVertexAttribArray(stage1->att_matrix + 2);
    glEnableVertexAttribArray(stage1->att_matrix + 3);

    GLuint frame;
    GLuint render;
    GLuint textures[3];

    glEnable(GL_TEXTURE_2D);
    glGenFramebuffers(1, &frame);
    glGenRenderbuffers(1, &render);
    glBindRenderbuffer(GL_RENDERBUFFER, render);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    glGenTextures(3, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_DIFFUSE]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_POSITION]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_NORMAL]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[TEXTURE_DIFFUSE], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textures[TEXTURE_POSITION], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, textures[TEXTURE_NORMAL], 0);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    glEnable(GL_DEPTH_TEST);

    GLfloat counter = 0.0f;

    GLenum deferred_targets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

    while (alive)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        counter += 0.0033f;

        glBindVertexArray(milky_way->format);
        glUseProgram(stage1->program);
        glUniform1f(stage1->uni_counter, counter);
        glBindFramebuffer(GL_FRAMEBUFFER, frame);
        glDrawBuffers(3, deferred_targets);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElementsInstanced(GL_TRIANGLES, 60, GL_UNSIGNED_INT, NULL, 3);
        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
        glDrawBuffer(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(stage2->format);
        glUseProgram(stage2->program);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        SwapBuffers(device);
    }

    cout << "Shutting down" << endl;

    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    glDeleteFramebuffers(1, &frame);
    glDeleteRenderbuffers(1, &render);
    glDeleteTextures(3, textures);
    glDisableVertexAttribArray(stage1->att_matrix);
    glDisableVertexAttribArray(stage1->att_matrix + 1);
    glDisableVertexAttribArray(stage1->att_matrix + 2);
    glDisableVertexAttribArray(stage1->att_matrix + 3);
    glUseProgram(GL_NONE);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    glDeleteBuffers(3, buffers);
    glBindVertexArray(GL_NONE);
    glDeleteVertexArrays(1, &array);

    delete milky_way;

    delete stage1;
    delete stage2;

cleanup_context:
    gl::wglMakeCurrent(device, NULL);
    gl::wglDeleteContext(context);

cleanup_window:
    /* Destroy window */
    ShowWindow(window, SW_HIDE);
    DestroyWindow(window);
    UnregisterClass(reinterpret_cast<LPCSTR>(atom), instance);

    gl::cleanup();

    return exit;
}

/* Shader methods */ /****************************************/

shader_stage1::shader_stage1()
{
    shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    shader_geometry = glCreateShader(GL_GEOMETRY_SHADER);
    shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();

    GLint length_vertex = strlen(source_vertex);
    GLint length_geometry = strlen(source_geometry);
    GLint length_fragment = strlen(source_fragment);

    glShaderSource(shader_vertex, 1, &source_vertex, &length_vertex);
    glShaderSource(shader_geometry, 1, &source_geometry, &length_geometry);
    glShaderSource(shader_fragment, 1, &source_fragment, &length_fragment);

    glCompileShader(shader_vertex);
    glCompileShader(shader_geometry);
    glCompileShader(shader_fragment);

    glAttachShader(program, shader_vertex);
    glAttachShader(program, shader_geometry);
    glAttachShader(program, shader_fragment);

    glBindFragDataLocation(program, GL_COLOR_ATTACHMENT0 - GL_COLOR_ATTACHMENT0, "r_color");
    glBindFragDataLocation(program, GL_COLOR_ATTACHMENT1 - GL_COLOR_ATTACHMENT0, "r_position");
    glBindFragDataLocation(program, GL_COLOR_ATTACHMENT2 - GL_COLOR_ATTACHMENT0, "r_normal");

    glLinkProgram(program);

    att_vertex = glGetAttribLocation(program, "att_vertex");
    att_color = glGetAttribLocation(program, "att_color");
    att_normal = glGetAttribLocation(program, "att_normal");
    att_matrix = glGetAttribLocation(program, "att_matrix");

    uni_perspective = glGetUniformLocation(program, "uni_perspective");
    uni_counter = glGetUniformLocation(program, "uni_counter");

    GLfloat matrix_perspective[] = {
        1.303225373f / (GetSystemMetrics(SM_CXSCREEN) / static_cast<GLfloat>(GetSystemMetrics(SM_CYSCREEN))), 0.0f, 0.0f, 0.0f,
        0.0f, 1.303225373f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.00020002f, -1.0f,
        0.0f, 0.0f, -0.200020002f, 0.0f
    };

    glUseProgram(program);

    glUniformMatrix4fv(uni_perspective, 1, GL_FALSE, matrix_perspective);
    glUniform1f(uni_counter, 0.0f);

    glUseProgram(GL_NONE);
}

shader_stage1::~shader_stage1()
{
    glUseProgram(GL_NONE);

    glDetachShader(program, shader_vertex);
    glDetachShader(program, shader_geometry);
    glDetachShader(program, shader_fragment);

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_geometry);
    glDeleteShader(shader_fragment);

    glDeleteProgram(program);
}

shader_stage2::shader_stage2()
{
    shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();

    GLint length_vertex = strlen(source_vertex);
    GLint length_fragment = strlen(source_fragment);

    glShaderSource(shader_vertex, 1, &source_vertex, &length_vertex);
    glShaderSource(shader_fragment, 1, &source_fragment, &length_fragment);

    glCompileShader(shader_vertex);
    glCompileShader(shader_fragment);

    glAttachShader(program, shader_vertex);
    glAttachShader(program, shader_fragment);

    glBindFragDataLocation(program, GL_COLOR_ATTACHMENT0 - GL_COLOR_ATTACHMENT0, "r_color");

    glLinkProgram(program);

    att_vertex = glGetAttribLocation(program, "att_vertex");

    uni_perspective = glGetUniformLocation(program, "uni_perspective");
    uni_color = glGetUniformLocation(program, "uni_color");
    uni_position = glGetUniformLocation(program, "uni_position");
    uni_normal = glGetUniformLocation(program, "uni_normal");

    GLfloat matrix_orthogonal[] = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -0.0020002f, 0.0f,
        -1.0f, -1.0f, -1.00020002f, 1.0f
    };

    glUseProgram(program);

    glUniformMatrix4fv(uni_perspective, 1, GL_FALSE, matrix_orthogonal);
    glUniform1i(uni_color, GL_TEXTURE0 - GL_TEXTURE0);
    glUniform1i(uni_position, GL_TEXTURE1 - GL_TEXTURE0);
    glUniform1i(uni_normal, GL_TEXTURE2 - GL_TEXTURE0);

    glUseProgram(GL_NONE);

    GLfloat vertices[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
    };

    GLuint indices[] = {
        1, 0, 2,
        3, 2, 0,
    };

    glGenVertexArrays(1, &format);

    glBindVertexArray(format);

    glGenBuffers(1, &buffer_vertices);
    glGenBuffers(1, &buffer_indices);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_indices);

    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(att_vertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    glEnableVertexAttribArray(att_vertex);

    glBindVertexArray(GL_NONE);
}

shader_stage2::~shader_stage2()
{
    glBindVertexArray(format);

    glDisableVertexAttribArray(att_vertex);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(GL_NONE);

    glDeleteBuffers(1, &buffer_vertices);
    glDeleteBuffers(1, &buffer_indices);

    glDeleteVertexArrays(1, &format);

    glUseProgram(GL_NONE);

    glDetachShader(program, shader_vertex);
    glDetachShader(program, shader_fragment);

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);

    glDeleteProgram(program);
}

/* Celestial methods */ /****************************************/

galaxy::galaxy(shader_stage1 *shader)
{
    this->shader = shader;

    GLfloat radius = (1.0f + sqrtf(5.0f)) / 2.0f;

    GLfloat vertices[] = {
          -1.0f,  radius,    0.0f,
           1.0f,    0.0f,    0.0f,
           1.0f,  radius,    0.0f,
           0.5f,    0.5f,    0.0f,
          -1.0f, -radius,    0.0f,
           0.0f,    1.0f,    0.0f,
           1.0f, -radius,    0.0f,
           0.0f,    0.5f,    0.5f,
           0.0f,   -1.0f,  radius,
           0.0f,    0.0f,    1.0f,
           0.0f,    1.0f,  radius,
           0.5f,    0.0f,    0.5f,
           0.0f,   -1.0f, -radius,
           1.0f,    0.0f,    1.0f,
           0.0f,    1.0f, -radius,
           1.0f,    1.0f,    0.0f,
         radius,    0.0f,   -1.0f,
           0.0f,    1.0f,    1.0f,
         radius,    0.0f,    1.0f,
           0.0f,    0.0f,    0.0f,
        -radius,    0.0f,   -1.0f,
           0.5f,    0.5f,    0.5f,
        -radius,    0.0f,    1.0f,
           1.0f,    1.0f,    1.0f,
    };

    GLuint indices[] = {
         0, 11,  5,
         0,  5,  1,
         0,  1,  7,
         0,  7, 10,
         0, 10, 11,
         1,  5,  9,
         5, 11,  4,
        11, 10,  2,
        10,  7,  6,
         7,  1,  8,
         3,  9,  4,
         3,  4,  2,
         3,  2,  6,
         3,  6,  8,
         3,  8,  9,
         4,  9,  5,
         2,  4, 11,
         6,  2, 10,
         8,  6,  7,
         9,  8,  1,
    };

    glGenVertexArrays(1, &format);

    glBindVertexArray(format);

    glGenBuffers(1, &buffer_vertices);
    glGenBuffers(1, &buffer_indices);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_indices);

    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 60 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(shader->att_vertex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(shader->att_normal, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(shader->att_color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLfloat *>(NULL) + 3);

    glEnableVertexAttribArray(shader->att_vertex);
    glEnableVertexAttribArray(shader->att_normal);
    glEnableVertexAttribArray(shader->att_color);

    glBindVertexArray(GL_NONE);
}

galaxy::~galaxy()
{
    glBindVertexArray(format);

    glDisableVertexAttribArray(shader->att_vertex);
    glDisableVertexAttribArray(shader->att_normal);
    glDisableVertexAttribArray(shader->att_color);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(GL_NONE);

    glDeleteBuffers(1, &buffer_vertices);
    glDeleteBuffers(1, &buffer_indices);

    glDeleteVertexArrays(1, &format);
}

/* Shader source codes */ /****************************************/

/* Stage 1, vertex shader: Adds rotation */
const GLchar *shader_stage1::source_vertex = "\
                                             #version 330 core\n\
                                             \n\
                                             in vec3 att_vertex;\n\
                                             in vec3 att_color;\n\
                                             in mat4 att_matrix;\n\
                                             in vec3 att_normal;\n\
                                             \n\
                                             out vec3 vg_color;\n\
                                             out vec3 vg_normal;\n\
                                             out vec3 vg_position;\n\
                                             out mat4 vg_matrix;\n\
                                             out mat4 vg_perspective;\n\
                                             \n\
                                             uniform mat4 uni_perspective;\n\
                                             uniform float uni_counter;\n\
                                             \n\
                                             void main()\n\
                                             {\n\
                                                 float spin = uni_counter * (-0.5 + 0.75 * gl_InstanceID);\n\
                                                 mat4 rotate = mat4(cos(spin), 0.0, -sin(spin), 0.0,\n\
                                                                          0.0, 1.0,        0.0, 0.0,\n\
                                                                    sin(spin), 0.0,  cos(spin), 0.0,\n\
                                                                          0.0, 0.0,        0.0, 1.0);\n\
                                                 vec4 position = uni_perspective * att_matrix * rotate * vec4(att_vertex, 1.0);\n\
                                                 \n\
                                                 gl_Position = position;\n\
                                                 vg_color = att_color;\n\
                                                 vg_normal = normalize(att_normal);\n\
                                                 vg_position = att_vertex;\n\
                                                 vg_matrix = att_matrix * rotate;\n\
                                                 vg_perspective = uni_perspective;\n\
                                             }\
                                             ";

/* Stage 1, geometry shader: Subdivides the icosahedron to approximate an icosphere */
const GLchar *shader_stage1::source_geometry = "\
                                                #version 330 core\n\
                                                \n\
                                                layout(triangles) in;\n\
                                                layout(triangle_strip, max_vertices = 8) out;\n\
                                                \n\
                                                in vec3 vg_normal[];\n\
                                                in vec3 vg_position[];\n\
                                                in vec3 vg_color[];\n\
                                                in mat4 vg_matrix[];\n\
                                                in mat4 vg_perspective[];\n\
                                                \n\
                                                out vec3 gf_normal;\n\
                                                out vec3 gf_position;\n\
                                                out vec3 gf_color;\n\
                                                \n\
                                                void main()\n\
                                                {\n\
                                                    vec3 position[3];\n\
                                                    vec3 normal[3];\n\
                                                    \n\
                                                    for (int i = 0; i < 3; i++)\n\
                                                    {\n\
                                                        position[i] = vec3((vg_position[i].x + vg_position[(i + 1) % 3].x) / 2.0,\n\
                                                                           (vg_position[i].y + vg_position[(i + 1) % 3].y) / 2.0,\n\
                                                                           (vg_position[i].z + vg_position[(i + 1) % 3].z) / 2.0);\n\
                                                        position[i] = normalize(position[i]) * (sqrt(10.0) + 2.0 * sqrt(5.0)) / 4.0;\n\
                                                        \n\
                                                        normal[i] = vec3((vg_normal[i].x + vg_normal[(i + 1) % 3].x) / 2.0,\n\
                                                                         (vg_normal[i].y + vg_normal[(i + 1) % 3].y) / 2.0,\n\
                                                                         (vg_normal[i].z + vg_normal[(i + 1) % 3].z) / 2.0);\n\
                                                    }\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(vg_position[0], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(vg_normal[0], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(vg_position[0], 1.0)).xyz;\n\
                                                    gf_color = vec3(1.0, 0.0, 0.0);\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[0], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[0], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[0], 1.0)).xyz;\n\
                                                    gf_color = vec3(1.0, 1.0, 0.0);\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[2], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[2], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[2], 1.0)).xyz;\n\
                                                    gf_color = vec3(1.0, 0.0, 1.0);\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[1], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[1], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[1], 1.0)).xyz;\n\
                                                    gf_color = vec3(0.0, 1.0, 1.0);\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(vg_position[2], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(vg_normal[2], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(vg_position[2], 1.0)).xyz;\n\
                                                    gf_color = vec3(0.0, 0.0, 1.0);\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    EndPrimitive();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[1], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[1], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[1], 1.0)).xyz;\n\
                                                    gf_color = vec3(0.0, 1.0, 1.0);\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(position[0], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(normal[0], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(position[0], 1.0)).xyz;\n\
                                                    gf_color = vec3(1.0, 1.0, 0.0);\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    gl_Position = vg_perspective[0] * vg_matrix[0] * vec4(vg_position[1], 1.0);\n\
                                                    gf_normal = (vg_matrix[0] * vec4(vg_normal[1], 0.0)).xyz;\n\
                                                    gf_position = (vg_matrix[0] * vec4(vg_position[1], 1.0)).xyz;\n\
                                                    gf_color = vec3(0.0, 1.0, 0.0);\n\
                                                    EmitVertex();\n\
                                                    \n\
                                                    EndPrimitive();\n\
                                                }\n\
                                            ";

/* Stage 1, fragment shader: Store results in framebuffers */
const GLchar *shader_stage1::source_fragment = "\
                                               #version 330 core\n\
                                               \n\
                                               in vec3 gf_color;\n\
                                               in vec3 gf_normal;\n\
                                               in vec3 gf_position;\n\
                                               \n\
                                               out vec3 r_color;\n\
                                               out vec3 r_normal;\n\
                                               out vec3 r_position;\n\
                                               \n\
                                               void main()\n\
                                               {\n\
                                                   r_color = gf_color;\n\
                                                   r_normal = gf_normal;\n\
                                                   r_position = gf_position;\n\
                                               }\
                                               ";

/* Stage 2, vertex_shader: Aligns rectangle to screen */
const GLchar *shader_stage2::source_vertex = "\
                                             #version 330 core\n\
                                             \n\
                                             in vec3 att_vertex;\n\
                                             \n\
                                             out vec2 vf_texcoord;\n\
                                             \n\
                                             uniform mat4 uni_perspective;\n\
                                             \n\
                                             void main()\n\
                                             {\n\
                                                 gl_Position = uni_perspective * vec4(att_vertex, 1.0);\n\
                                                 vf_texcoord = att_vertex.xy;\n\
                                             }\n\
                                             ";

/* Stage 2, fragment shader: Displays framebuffers and composited result */
const GLchar *shader_stage2::source_fragment = "\
                                               #version 330 core\n\
                                               \n\
                                               in vec2 vf_texcoord;\n\
                                                     \n\
                                               out vec4 r_color;\n\
                                               \n\
                                               uniform sampler2D uni_color;\n\
                                               uniform sampler2D uni_position;\n\
                                               uniform sampler2D uni_normal;\n\
                                               \n\
                                               void main()\n\
                                               {\n\
                                                   vec2 texcoord = vf_texcoord * 2.0;\n\
                                                   \n\
                                                   if (vf_texcoord.s >= 0.5)\n\
                                                       if (vf_texcoord.t >= 0.5)\n\
                                                           r_color = texture(uni_position, texcoord);\n\
                                                       else\n\
                                                       {\n\
                                                           vec3 light = normalize(vec3(-10.0, 0.0, -5.0) - texture(uni_position, texcoord).xyz);\n\
                                                           r_color = clamp(max(dot(light, texture(uni_normal, texcoord).xyz), 0.0) * texture(uni_color, texcoord), 0.0, 1.0);\n\
                                                       }\n\
                                                   else if (vf_texcoord.t >= 0.5)\n\
                                                       r_color = texture(uni_color, texcoord);\n\
                                                   else\n\
                                                       r_color = texture(uni_normal, texcoord);\n\
                                               }\n\
                                               ";