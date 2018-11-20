#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

// max eingabe laenge
#define MAX 255

// Information
char i_author[] = { "Author      : Nhut Hoa, Huynh" };
char i_version[] = { "Version     : 1.0" };

int main(void) {
//	Eingabe erstellen
	char eingabe[MAX];

//	Abbruchbedingung erstellen
	_Bool beenden = 0;

//	Programmdurchlauf beenden, wenn Abbruchbedingung "exit != 0"
	while (beenden == 0) {

//		Prompt-String erstellen
		printf("%s - Was willst du %s? ", getenv("PWD"), getenv("USER"));

//		Eingabeaufforderung starten
		fgets(eingabe, MAX, stdin);

//		Zeilenumbruch entfernen
		eingabe[strlen(eingabe) - 1] = '\0';
		int len_eingabe = strlen(eingabe);

//		Befehlsabrage
		if (strcmp(eingabe, "Quit") == 0) { 				//<-- Programm beenden
			beenden = 1; 						//<-- Abbruchbedingung

		} else if (strcmp(eingabe, "version") == 0) { 			//<-- Programm- Inforation ausgeben
			printf("%s\n%s\n", i_author, i_version);

		} else if (eingabe[0] == '/') { 				// Pfad neu erstellen?
			if (chdir(eingabe) == -1) { 				//Fehlermeldung
				puts("Arbeitsverzeichnis konnte nicht gefunden werden.");
			} else { 						//<-- Pfad neu erstellen
				setenv("PWD", eingabe, MAX - 1);
			}

		} else if (strcmp(eingabe, "Help") == 0) { 			//Built in- Befehle mit Info ausgeben
			puts("Befehl: Quit = Programm beenden");
			puts("Befehl: version = Information ueber Author und aktuelle version");
			puts("Befehl: / = setzte Pfad");
			puts("Befehl: Help = Information und Beschreibung über Builin");

		} else {
//			Eingabe aufarbeitung mit &
			_Bool im_hintergrund = 0;
			if (eingabe[len_eingabe - 1] == '&') {
				im_hintergrund = 1;
				eingabe[len_eingabe - 1] = '\0';
			}

//			Kein "Built-In" - Befehl: Prozess erzeugen
			pid_t pid;
			switch (pid = fork()) {
			case -1:						//Fehler bei fork()
				perror("fork()");
				return EXIT_FAILURE;
//				break;
			case 0:							//Kindprozess
				if (execlp(eingabe, eingabe, NULL) == -1) {
					printf("Komando \"%s\" konnte nicht gefunden werden!\n", eingabe);
				}
//				Prozess beenden, wenn Prozess fertig
				beenden = 1;
				break;
			default:						//Elternprozess
				if (im_hintergrund == 1) {
					waitpid(pid, NULL, WNOHANG); 		//Elternprozess wird im Hintergrund ausgeführt
				} else {
					waitpid(pid, NULL, 0);			//Elternprozess wartet auf Kindprozess
				}
				break;
			}
		}
	}
	return EXIT_SUCCESS;
}

