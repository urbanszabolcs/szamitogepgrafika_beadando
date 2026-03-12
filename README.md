# Project: [Név kitalálás alatt]

## 1. A projekt leírása

### A, Játékmenet
A játékos egy hajó kapitányának szerepébe bújik egy **procedurálisan generált** tengeren, ahol a szigetek elhelyezkedése minden játékalkalommal változik. 

* **Cél:** A navigáció elsajátítása, az erőforrások kezelése és a kijelölt küldetések teljesítése.
* **Játéktér:** Egy $X \times X$ kiterjedésű terület, amelyen belül különböző stratégiai pontokat kell elfoglalni vagy felfedezni.
* **Dinamikus környezet:** Valós idejű napszakváltás és időjárási rendszerek befolyásolják a látási viszonyokat és a hajó irányíthatóságát:
    * **Napszakok:** Reggel, Nappal, Este.
    * **Időjárás:** Tiszta idő, Köd, Eső és Vihar.

---

### B, Megvalósítás
* **Kétmódú kamerarendszer:**
    * **TPS nézet:** A kamera rögzítve követi a hajó mozgását (külső nézet).
    * **Szabad nézet:** A `WASD` billentyűk segítségével a kamera leválasztható, így a virtuális tér szabadon bejárható.
* **Objektumok:**
    * Interaktív hajómodell.
    * Forgatható ágyútornyok.
    * Környezeti elemek (sziklák, szigetek, bóják).
* **Animációk:**
    * A vízfelszín hullámzása (Shader alapú).
    * A hajó dőlése a kanyarodás és a hullámok függvényében.
    * Ágyúk visszarúgása és torkolattűz effektek.

---

### C, Többletfunkciók
* **Fejleszthető hajótest:** Sebesség, páncélzat és tűzerő növelése.
* **Legénység kezelése:** Különböző bónuszokat adó tisztek felfogadása.
* **Kereskedelmi rendszer:** Áruk szállítása a szigetek között a profit reményében.


### D, Példa GUI
| Elem | Funkció | Elhelyezkedés |
| :--- | :--- | :--- |
| **Iránytű** | Navigáció segítése | Fent, középen |
| **Sebességmérő** | Hajó aktuális sebessége (csomó) | Bal alsó sarok |
| **Életerő (Hull)** | Hajótest állapota | Bal alsó sarok |
| **Mini-map** | Procedurális területnézet | Jobb alsó sarok |
| **Lőszer/Raktér** | Erőforrások kijelzése | Jobb felső sarok |
