		README

Tema1 Elemente de Grafica pe Calculator - Flappy Bird
Budau George 332 CC


	Proiectul prezinta o implementare proprie a versiunii jocului Flappy Bird.
Pasarea, care este formata prin combinarea unor figuri geometrice, creeaza impresia ca se misca pe ecran printre perechi 
de tevi, reprezentate prin dreptunghiuri, de la stanga la dreapta. La fiecare apasare a tastei "space" se initiaza un 
salt asupra pasarii. Scopul jocului este ca pasarea sa treaca printre perechile de dreptunghiuri care apar pe ecran, 
jucatorul acumuland un punctaj cat mai mare. Pasarea este afectata de gravitatie si va avea intotdeauna tendinta sa cada.
Apasati SPACE pentru a porni jocul.

	Detalli de implementare:
-caderea/ridicarea pasarii este implementata prin translatie pe axa Oy
-perecihle de dreptunghiuri sunt translatate incontinuu pe axa Ox (de la dreapta la stanga). De fiecare data cand o pereche
de dreptunghiuri dispare (s-a deplasat spre stanga pana cand a iesit din spatiul de desenare), apare o alta pereche de 
dreptunghiuri in partea dreapta. Initial sunt create 4 perechi de dreptunghiuri, cu distanta intre ele constanta 
si inaltimi de fiecare data diferite si salvate intr-un vector; atunci cand o pereche translatata iese din ecran, celelalte 
3 perechi sunt trase spre stanga in locul initial al vechii perechi, pastrandu-si inaltimile, iar cea de-a 4 pereche va 
primi o inaltime aleatoare. Astfel consumul de memorie si timpul de redare sunt optimizate.
-pasarea este aproximata prin cercul care o incadreaza astfel ca se poate face o verificare pentru coliziunea intre un cerc 
si dreptunghiuri; coliziunea este verificata astfel: se identifica cea mai apropiata latura a dreptunghiului fata de pasare si
se calculeaza distanta dintre centrul pasarii si cel mai apropiat punct de pe latura gasita; daca rezultatul obtinut este mai
mic decat raza cercului atunci s-a produs coliziune.
-jocul porneste doar dupa prima apasare a tastei SPACE, pana atunci pasarea va aparea pe ecran realizand o translatatie scurta 
pe OY, lasand impresia ca zboara la o inaltime constanta.
-scorul este incrementat dupa ce pasarea trece complet printr-o pereche de dreptunghiuri, fiind afisat in timp real la consola
-jocul se sfarseste cand se produce coliziune, afisandu-se la consola punctajul final obtinut
-cand jocul se sfarseste, fundalul se blocheaza in pozitia in care a fost la momentul impactului, iar pasarea cade pana la 
iesirea din ecran, printr-o translatie pe axa OY.
	
	
	
	