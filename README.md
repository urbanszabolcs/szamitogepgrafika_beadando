# Project: Boating Around

## 1. A projekt leírása

### A, Játékmenet
A játékos egy hajó kapitányának szerepébe bújik egy **procedurálisan generált** 3D-s tengeren, ahol a környezet minden játékalkalommal egyedi.

* **Fizika és Irányítás:** "War Thunder" stílusú tengerészeti fizika. A hajónak tehetetlensége van (momentum), a billentyűzet a gázkart vezérli (előre/hátra), és a kanyarodás sugara a hajó aktuális sebességétől függ.
* **Cél:** A navigáció elsajátítása a szigetek között, elkerülve az ütközéseket. (Ha a hajó szigetnek ütközik, sérülést szenved. 0 HP esetén a játék újraindul).
* **Dinamikus környezet:** Valós idejű, dinamikus 3-fázisú napszakváltás (Nappal, Naplemente, Éjszaka) folyamatosan változó égbolt-színekkel, árnyékokkal és Holdfénnyel. A beépített "időgép" funkcióval az idő felgyorsítható vagy visszafordítható.

---

### B, Megvalósítás (Tech Stack & Rendszerek)
* **Grafika és Motor:** C nyelven, natív OpenGL (Immediate Mode) és SDL2 használatával.
* **Kamera:** 3D-s követőkamera (TPS), amely reagál az egér mozgására és a görgővel zoomolható.
* **Procedurális Szigetek:** Szinuszhullám-alapú (Sine Wave Deformation) algoritmus generál organikus, nem-szabályos partszakaszokat.
* **Víz Rendering:** 3D-s rács (grid), amelyet több szinusz/koszinusz hullám mozgat. A hajó automatikusan felveszi a víz magasságát.
* **Optimalizáció:** Távolságalapú renderelés (Distance Culling) biztosítja, hogy csak a ködön belüli (`FOG_START - FOG_END`) víz- és sziget-vertexek terheljék a processzort, garantálva a magas FPS-t.
* **Egyedi 2D GUI Rendszer:** Skálázható virtuális HUD tér (800x700), beépített menürendszerrel és dinamikus betűtípus-rendereléssel (`SDL_ttf`).

---

### C, Irányítás és GUI

**Gombok:**
* `W` / `S`: Gázkar (Előre/Hátra)
* `A` / `D`: Kormánylapát (Csak haladás közben működik)
* `Egér` & `Görgő`: Kamera forgatása és Zoom
* `F1`: Útmutató (Guide) menü megnyitása/bezárása
* `ESC`: Beállítások (Felbontás váltás / Kilépés)
* `Numpad +` / `-`: Idő előretekerése / Visszatekerése

**HUD Elemek:**
| Elem | Funkció | Elhelyezkedés |
| :--- | :--- | :--- |
| **Életerő (Hull)** | Hajótest állapota (Ütközéskor csökken) | Bal alsó sarok |
| **Sebesség & Váltó** | Hajó aktuális sebessége (Knot) és irány (Ahead/Reverse) | Bal alsó sarok |
| **Iránytű** | Forgó navigációs segédlet | Jobb felső sarok |

---

## 2. Kreditek és Források (Credits & Assets)

A projekt elkészítése során az alábbi külső eszközök és modellek kerültek felhasználásra:

* **Játékos Hajó (Cruise Liner 3D Modell):** * A hajó 3D modelljét és textúráját a Poly Pizza oldaláról töltöttük le.
  * Forrás/Készítő: [https://poly.pizza/m/dgLCxDWhnZQ](https://poly.pizza/m/dgLCxDWhnZQ)
* **Grafikai Könyvtárak:** OpenGL, SDL2, SDL2_ttf (szövegmegjelenítés), SDL2_image (textúrák betöltése).# Project: boating around

