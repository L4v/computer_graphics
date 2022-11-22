#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

//Kanali (in, out, uniform)
layout(location = 0) in vec2 inPos; //Pozicija tjemena
layout(location = 1) in vec4 inCol; //Boja tjemena - ovo saljemo u fragment sejder
out vec4 channelCol; //Izlazni kanal kroz koji saljemo boju do fragment sejdera

void main() //Glavna funkcija sejdera
{
	gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0); //gl_Position je predefinisana promjenljiva za pozicije u koju stavljamo nase koordinate. Definisana je kao vec4 pa zbog toga konvertujemo
	channelCol = inCol;
}