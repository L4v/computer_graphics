# Računarska grafika
Jezik: [:gb:](README.md)  
Materijal za Računarsku grafiku na Fakultetu tehničkih nauka.

## Sadržaj
- [Računarska grafika](#računarska-grafika)
	- [Sadržaj](#sadržaj)
	- [Grane](#grane)
	- [Okruženje](#okruženje)
	- [Instalacija i podešavanje](#instalacija-i-podešavanje)
		- [Windows](#windows)
			- [Podešavanje Visual Studio-a](#podešavanje-visual-studio-a)
			- [Base starter projekat](#base-starter-projekat)
			- [Napravite svoj projekat](#-napravite-svoj-projekat)
		- [Linux](#linux)
			- [Arch bazirani](#arch-bazirani)
			- [Debian bazirani](#debian-bazirani)
			- [Kompajliranje i pokretanje](#kompajliranje-i-pokretanje)

## Grane
- base: Osnovni kod, starter projekat
- demo: Demo šta je moguće izvršiti u OpenGL-u posle polaganja predmeta
- week-x: Materijal za nedelju x
- additional_materials: Sadrži dodatne materijale za učenje

## Okruženje
> Napomena: Ukoliko želite da koristite druge biblioteke i tehnologije, konsultujte se sa Jovan Ivošević na adresi ivosevic.jovan@uns.ac.rs.
- GLFW
- GLEW
- GLM
- Assimp

## Instalacija i podešavanje
### Windows
Ukoliko želite da koristite projekat sa već podešenim paketima, možete koristiti verziju sa GiM-a ili GitHub-a.
#### Podešavanje Visual Studio-a
Odaberite "Desktop development with C++" u Visual Studio Installer-u
![Desktop development with C++](/img/desktop_development_cpp.png)
#### Base starter projekat
- Sa "base" grane, skinite starter projekat
- Otvorite u Visual Studio i proverite NuGet pakete, možda je potrebno da se "restore"
	- ![NuGet](/img/nuget0.png)
	- ![NuGet Restore](/img/nuget_restore.png)
- Posle toga ste spremni za dalje! Dalje podešavanje neće biti potrebno. Ukoliko naletite na problem, molimo Vas da nas kontaktirate.
#### Napravite svoj projekat
- Napravite novu C/C++ konzolnu aplikaciju u Visual Studio
- Sa selektovanim .cpp file open, odaberite "Project" u toolbar-u
- Odaberite Manage "NuGet Packages..."
	- ![NuGet](/img/nuget0.png)
- Odaberite "Browse" tab i ukucajte ime paketa u pretrazi
- Za GLFW unesite "glfw" i instalirajte paket čiji su autori Marcus Geelnard i Camilia Berglund
	- ![NuGet](/img/nuget1.png)
- Na isti način instalirajte sledeće NuGet pakete:
	- glew-2.2.0 by NerdTronik
	- glm by GLM contributors (versions 0.9.9.800 and higher)
### Linux
Mogu postojati razlike u zavisnosti od distribucije koju koristite. Kontaktirajte ivosevic.jovan@uns.ac.rs kako bi ažurirao.
- Napravite direktorijum za Vaš kod (možete koristiti .cpp i .hpp fajlove sa repozitorijuma)
#### Arch bazirani
> Manjaro, EndeavourOS, etc...

- Da bi instalirali GLM, GLFW, GLEW i Assimp:
```bash
sudo pacman -S glm glfw glew assimp
```
#### Debian bazirani
> Ubuntu, Pop, Deepin, Kali

```bash
sudo apt install libglfw3 libglfw3-dev libglm-dev libglew-dev assimp
```

#### Kompajliranje i pokretanje
Možete koristiti make, cmake ili bilo koji drugi alat, ali za većinu slučajeva sledeća komanda će biti dovljna:
```bash
# To compile (run this command inside your code dir, where all the .cpp and .hpp files are). NOTE: pkg-config is surrounded by backticks `, not by quotes '
g++ -o <your_program_name> *.cpp `pkg-config --libs glfw3 glew assimp` -ldl
# To run
./<your_program_name>
```
Možete prekopirati ovaj kod u .sh fajl, npr: build.sh i pokrenuti `chmod +x build.sh` samo prvi put kako bi fajl postao izvršiv, a zatim za pokretanje kucajte: `./build.sh` 
