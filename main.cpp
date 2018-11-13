#include <functional>

#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <time.h>       /* time */
#include <cmath>

// Shader sources
const GLchar* vertexSource =
    "attribute vec4 position;                     \n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_Position = vec4(position.xyz, 1.0);     \n"
    "}                                            \n";
const GLchar* fragmentSource =
    "precision mediump float;\n"
    "uniform vec3 u_BackgroundColor;\n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_FragColor = vec4(u_BackgroundColor,1.0);    \n"
    "}                                            \n";


const int kNumVertices = 8;

class Box
{
public:
	Box(float inWidth=0.05, float inHeight=0.05) : xPos(0.0), yPos(0.0), fWidth(inWidth), fHeight(inHeight) { 
		vertices = new GLfloat[kNumVertices]; 

		glGenBuffers(1, &fVBO);
	    glBindBuffer(GL_ARRAY_BUFFER, fVBO);

		// Create an element array
	    glGenBuffers(1, &fEBO);

	    GLuint elements[] = {
	        0, 1, 3,
	        2, 3, 0,
	    };

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fEBO);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	}

	virtual ~Box(){ delete []vertices; }

	void Reset()
	{
		xPos = 0;
		yPos = 0;
	}

	void Move(float inX, float inY)
	{
		xPos += inX;
		yPos += inY;

		vertices[0] = -fWidth+xPos;
		vertices[1] = fHeight+yPos;
		vertices[2] = fWidth+xPos;
		vertices[3] = fHeight+yPos;
		vertices[4] = -fWidth+xPos;
		vertices[5] = -fHeight+yPos;
		vertices[6] = fWidth+xPos;
		vertices[7] = -fHeight+yPos;

	}

	void Draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, fVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*kNumVertices, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fEBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	bool FlipX() {	return (xPos < -1.0 || xPos > 1.0);}
	bool FlipY() {	return (yPos < -1.0 || yPos > 1.0); }
	bool IsIntersecting(Box* inBox)
	{
		bool bIntersecting = false;
		if (std::abs(inBox->GetXPos()) > 0.75 && inBox->GetYPos() < (fHeight+yPos) 
			&& inBox->GetYPos() > (-fHeight+yPos))
			bIntersecting = true;
		return bIntersecting;
	}

	float GetXPos() {return xPos;}
	float GetYPos() {return yPos;}
	float GetWidth() {return fWidth;}
	float GetHeight() {return fHeight;}
	GLfloat* GetVertices() {return vertices;}
private:
	GLfloat* vertices;
	float xPos;
	float yPos;
	float fWidth;
	float fHeight;
	GLuint fVBO;
	GLuint fEBO;
};

// an example of something we will control from the javascript side
bool background_is_black = true;
float bar_one_y = 0.00;
float bar_two_y = 0.00;

// the function called by the javascript code
extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }

extern "C" void EMSCRIPTEN_KEEPALIVE move_bar_one_down() 
{ 
	bar_one_y -= 0.01;
}

extern "C" void EMSCRIPTEN_KEEPALIVE move_bar_one_up() 
{ 
	bar_one_y += 0.01;
}

extern "C" void EMSCRIPTEN_KEEPALIVE move_bar_two_down() 
{ 
	bar_two_y -= 0.01;
}

extern "C" void EMSCRIPTEN_KEEPALIVE move_bar_two_up() 
{ 
	bar_two_y += 0.01;
}

std::function<void()> loop;
void main_loop() { loop(); }

int main()
{
    SDL_Window *window;
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Box* ball = new Box();
    ball->Move(0.0, 0.0);

    Box* bar1 = new Box(0.03,0.3);
   	bar1->Move(0.85, 0.0);

   	Box* bar2 = new Box(0.03,0.3);
   	bar2->Move(-0.85, 0.0);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    std::srand (time(NULL));
    float xMove = ((float)(std::rand()%10)+2)/1000.0*(std::rand()%2==2 ? 1.0 : -1.0);
    float yMove = ((float)(std::rand()%10)+2)/1000.0*(std::rand()%2==2 ? 1.0 : -1.0);

    loop = [&]
    {		    
        // Clear the screen
        GLint background = glGetUniformLocation(shaderProgram, "u_BackgroundColor");
        if( background_is_black )
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glUniform3f(background, 1.0, 1.0, 1.0);
        }
        else
        {
            glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
            glUniform3f(background, 0.0, 0.0, 0.0);
        }
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the pong ball
        if (ball->FlipX())
        {
        	ball->Reset();
        	xMove = ((float)(std::rand()%10)+2)/1000.0*(std::rand()%2==2 ? 1.0 : -1.0);
    		yMove = ((float)(std::rand()%10)+2)/1000.0*(std::rand()%2==2 ? 1.0 : -1.0);
        }
        if (ball->FlipY())
        	yMove *= -1;
        if (bar1->IsIntersecting(ball) || bar2->IsIntersecting(ball))
        	xMove *= -1;
		ball->Move(xMove, yMove);
        ball->Draw();
        bar1->Move(0.0, bar_one_y);
        bar1->Draw();
        bar2->Move(0.0, bar_two_y);
        bar2->Draw();

        SDL_GL_SwapWindow(window);
    };

    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}
