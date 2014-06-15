Wersja 0.99 kompilatora na Języki Formalne i Techniki Translacji.

Jak używać?
Wejście kompilatora odbywa się przez standardowe wejście; wyjście odbywa się przez standardowe wyjście.
Przykład użycia: c < program0.imp > wyjscie.out
Output bezpośrednio do interpretera: c < test.imp > test.out.txt && interpreter test.out.txt


ZMIANY względem ostatniej wersji: 
- naprawienie buga z generowaniem wartości w rejestrze a
- zmiana generowania wartości w rejestrze a z liniowej inkrementacji na logarytmiczną kombinację inc-shl
- zniesienie ograniczenia na wielkość liczby
- naprawienie buga z dzieleniem przez zero

1. Funkcjonalności:
Zawarte są wszystkie funkcje podstawowe, które powinien obejmować ten kompilator - cała gramatyka jest "spełniona".

2. Rzeczy do zmiany:
Póki co niektóre rzeczy są robione bardzo brzydko, 
- dość brzydko zrealizowane dzielenie
- nie zapisuję stałych w rejestrze p, jedynie jest 'zastępowana' przez liczbę którą reprezentuje; niepotrzebne generowanie 
- brak sprawdzenia czy wartość zmiennej została zdefiniowana
- wiadomości o błędach...


3. Co ciekawego?
a. szybkie mnożenie (pisemne)

