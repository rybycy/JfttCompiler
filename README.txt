Wersja 0.99 kompilatora na J�zyki Formalne i Techniki Translacji.

Jak u�ywa�?
Wej�cie kompilatora odbywa si� przez standardowe wej�cie; wyj�cie odbywa si� przez standardowe wyj�cie.
Przyk�ad u�ycia: c < program0.imp > wyjscie.out
Output bezpo�rednio do interpretera: c < test.imp > test.out.txt && interpreter test.out.txt


ZMIANY wzgl�dem ostatniej wersji: 
- naprawienie buga z generowaniem warto�ci w rejestrze a
- zmiana generowania warto�ci w rejestrze a z liniowej inkrementacji na logarytmiczn� kombinacj� inc-shl
- zniesienie ograniczenia na wielko�� liczby
- naprawienie buga z dzieleniem przez zero

1. Funkcjonalno�ci:
Zawarte s� wszystkie funkcje podstawowe, kt�re powinien obejmowa� ten kompilator - ca�a gramatyka jest "spe�niona".

2. Rzeczy do zmiany:
P�ki co niekt�re rzeczy s� robione bardzo brzydko, 
- do�� brzydko zrealizowane dzielenie
- nie zapisuj� sta�ych w rejestrze p, jedynie jest 'zast�powana' przez liczb� kt�r� reprezentuje; niepotrzebne generowanie 
- brak sprawdzenia czy warto�� zmiennej zosta�a zdefiniowana
- wiadomo�ci o b��dach...


3. Co ciekawego?
a. szybkie mno�enie (pisemne)

