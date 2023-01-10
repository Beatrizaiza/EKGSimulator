# EKGSimulator
# Algorithm AF3

Algorithmus AF3:
Seien x[n] der n-te abgetastete Wert des EKG-Signals und y[n] = x[n+1]-x[n-1].
Falls y[n] und y[n+1] beide den Wert 800 überschreiten, so liegt bei n ein Kandidat für den QRS-Komplex vor. Der Kandidat wird als erkannter QRS-Komplex bewertet, wenn der letzte detektierte QRS-Komplex mehr als 100 ms zurückliegt. Andernfalls wird der Kandidat verworfen.

Variante 1: Implementierung unter Verwendung von ecgData
ecgData enthält die mit 125 Hz abgetasteten digitalisierten Werte eines EKGs. Ihr Programm soll
alle 8 ms das nächste Array-Element x auslesen. Erreicht der Index den Maximalwert, so soll mit
Index 0 fortgesetzt werden. Bei jedem neuen Element wird geprüft, ob ein Kandidat für einen QRSKomplex vorliegt. Falls ein QRS-Komplex gefunden wurde, soll die Pulsfrequenz über den zeitlichen
Abstand zum vorhergehenden QRS-Komplex ermittelt und auf dem LCD-Display ausgegeben
werden. Das Herz-Symbol soll mit der Ausgabe ein- und nach 100 ms wieder ausgeschaltet werden.

---

### AF3

Analysieret ob Signal QRS komplex ist oder nicht.

n=1

x[0] = ecgData[0] = 860

x[1] = ecgData[1] = 696

x[2]= ecgData[2] = 677

x[3]= ecgData[3] = 635

x[4]= ecgData[4] = 537

**y[n] = x[n+1] - x[n-1]**

y[1] = 677-696 = -19

n=2

y[2] = 696-537 = 159

if (y1>800 && y2>800){ 

return true;

} FALSE

**Soll de Af3 als funktion geschrieben werden oder im main?**

AF3

```c
int xvor, xnach, , y, ynext;

for(n=1,n≤dataLen,n++){

xvor = ecgData[n-1];

xnach = ecgData[n+1];

y1= xnach-xvor;

n++;

xvor = ecgData[n-1];

xnach = ecgData[n+1];

y2= xnach-xvor;

}
```

als fuktion:

```c
// Algorithmus AF3 -??????
int af3 (unsigned int a){
		
		for(a=1;a≤dataLen;a++){

        xvor = ecgData[a-1];

        xnach = ecgData[a+1];

        y1= xnach-xvor;

        n++;

        xvor = ecgData[a-1];

        xnach = ecgData[a+1];

        y2= xnach-xvor;

        if (y1>800 && y2>800)
        {
            isTrue= 1;
        }
        else
            isTrue = 0;

    }

    return isTrue;
}

```

---

**Pulsfrequenz:** über den zeitlichen Abstand zum vorhergehenden QRS-Komplex ermittelt und auf dem LCD-Display ausgegeben werden.

puls= 60000/abstand;

---

**********************QRS Abstand Berechnen:**********************  

lösung 1:

wenn jede 8 ms ein interrupt routine durchgefuhrt wird und abstand muss grösser als 100ms (d.h 100ms/8ms = mindest 12,5 interrupt routine danach) dann n muss grosser n+12 sein.

```c
if (y1>800 && y2>800)
{
	int newIndex = n;
	if(newIndex > oldIndex+12)
	{
			LCD_Symbol(LCD_HRT_ID,LCD_SYMBOL_ON); 
			abstand = newIndex-oldIndex;
			int puls= 60000/abstand;
			LCD_displayShort(puls);
			newIndex = oldIndex;
	}
}

```

---

**Das Herz-Symbol soll mit der Ausgabe ein- und nach 100 ms wieder ausgeschaltet werden:**

(LCD_Hrt_ID) mit Timer möglich?

---

**Der Kandidat wird als erkannter QRS-Komplex bewertet, wenn der letzte detektierte QRS-Komplex mehr als 100 ms zurückliegt.**

**************************Kann man es mit 2 unabhangig Timers machen?**************************

Timer statt von Watchdog benutzen?

# Variante 2

### Implementierung unter Verwendung des Analogeingangs

Das EKG liegt nun als analoges Signal am Kanal A10 (P9.2) an. Verwenden Sie als Referenzspannung 2.5 V (Register REFCTL0). Konfigurieren Sie P9SEL0 und P9SEL1 so, dass P9.2 als A10 verwendet
wird (siehe MSP430FR6989 Data Sheet).
Alle 8 ms soll ein neuer Wert digitalisiert werden. Wie bei Schritt 1 soll geprüft werden, ob ein QRSKomplex vorliegt und ggf. die Ausgabe der Pulsfrequenz erfolgen. 

**Bei geschickter Implementierung müssen Sie lediglich die Erfassung des nächsten Messwerts anpassen.**
Beide Varianten sollen durch bedingte Kompilierung in einer Quelldatei implementiert werden.
Führen Sie dazu einen sog. Compilerschalter
#define USE_ADC
ein, über den Sie zwischen den beiden Varianten unterscheiden, z.B.
#ifndef USE_ADC
xNew = ecgData[index++];
#else
// AD-Wandler starten,
// xNew den neuen AD-Wert zuweisen
#endif
Das Analogsignal erzeugt ein LaunchPad vom Typ MSP-EXP430FR4133. Dieses müssen Sie nicht
programmieren, sondern lediglich einstellen. Die USB-Verbindung dient lediglich der Spannungsversorgung. Mit Button S2 (rechts) stellen Sie den Modus so ein, dass das HERZ-Symbol an ist. Mit
Button S1 (links) können Sie nun zwischen drei Geschwindigkeiten (48/60/75) wählen:
Abbildung 3: MSP-EXP430FR4133-LaunchPad
Verbinden Sie per Kabel den Ausgang VA auf dem grünen DAC-Board (siehe Abbildung 3) mit dem
Pin 9.2. Das EKG- Signal liegt nun in einer Endlosschleife am analogen Eingang des MSPEXP430FR6989 LauchPad an. Wie bei Variante 1 soll der QRS-Komplex bestimmt und die ermittelte Pulsfrequenz angezeigt werden.
