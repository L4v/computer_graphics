#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

//Kanali (in, out, uniform)
in vec4 channelCol; //Kanal iz Verteks sejdera - mora biti ISTOG IMENA I TIPA kao u vertex sejderu
out vec4 outCol; //Izlazni kanal koji ce biti zavrsna boja tjemena

void main() //Glavna funkcija sejdera
{
	outCol = channelCol;
}