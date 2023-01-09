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
