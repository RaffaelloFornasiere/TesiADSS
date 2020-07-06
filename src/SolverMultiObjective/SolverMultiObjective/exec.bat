:: PARAMETRI OBBLIGATORI
:: a "-c" segue il nome del file contenente il circuito
:: a "-l" segue il nome del file contenente la libreria delle celle
:: a "-f" segue il nome del file sul quale verrano stampati i valori di output. Se pari a "." allora
:: verrà stampato su standard output
:: a "-p" segue il numero di individui della popolazione
:: a "-e" segue il numero di individui elite
:: a "-m" segue il numero di mutanti
:: a "-r" segue il valore di rho_e

:: PARAMETRI FACOLTATIVI
:: a "-g" segue il valore generazioni minime/massime che l'algoritmo deve fare evolvere
:: a "-d" segue il valore di deadlock
:: a "-t" segue il valore di delay che si vuole raggiungere
:: a "-b" segue il valore di bias sull'area
:: "-a" indica che gli stop criteria devono essere messi in And, se non presente 
:: vegono messi in Or
:: DEVE ESSERE IMPOSTATO ALMENO UN CRITERIO DI ARRESTO 
:: Se tutti i criteri inseriti hanno valore 0 allora l'algoritmo
:: non si fermerà mai. 

main 	-c ../../../files/InputFiles/Circuits/deco6.vhdl ^
		-l ../../../files/InputFiles/Cell_Libraries/my_cell_library.hs ^
		-p 100 -g 50 -m 0.05
 
