AUTOR: Micha³ Robaszyñski 194165

Wersja 0.99 kompilatora na Jêzyki Formalne i Techniki Translacji.

Jak u¿ywaæ?
Wejœcie kompilatora odbywa siê przez standardowe wejœcie; wyjœcie odbywa siê przez standardowe wyjœcie.
Przyk³ad u¿ycia: c < program0.imp > wyjscie.out
Output bezpoœrednio do interpretera: c < test.imp > test.out.txt && interpreter test.out.txt


ZMIANY wzglêdem ostatniej wersji: 
- naprawienie buga z generowaniem wartoœci w rejestrze a
- zmiana generowania wartoœci w rejestrze a z liniowej inkrementacji na logarytmiczn¹ kombinacjê inc-shl
- zniesienie ograniczenia na wielkoœæ liczby
- naprawienie buga z dzieleniem przez zero

1. Funkcjonalnoœci:
Zawarte s¹ wszystkie funkcje podstawowe, które powinien obejmowaæ ten kompilator - ca³a gramatyka jest "spe³niona".

2. Rzeczy do zmiany:
Póki co niektóre rzeczy s¹ robione bardzo brzydko, 
- doœæ brzydko zrealizowane dzielenie
- nie zapisujê sta³ych w rejestrze p, jedynie jest 'zastêpowana' przez liczbê któr¹ reprezentuje; niepotrzebne generowanie 
- brak sprawdzenia czy wartoœæ zmiennej zosta³a zdefiniowana
- wiadomoœci o b³êdach...


3. Co ciekawego?
a. szybkie mno¿enie (pisemne)

