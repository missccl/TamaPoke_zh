#include "i18n.h"
#include "pet.h"        // MED_COUNT
#include <Preferences.h>

Lang gLang = LANG_DEFAULT;

// Tabla de cadenas [idioma][id]. Sin acentos ni enes: la fuente bitmap del
// firmware no los tiene (por eso el espanol ya iba "Esta", "bano", etc.).
static const char *const STRINGS[LANG_COUNT][STR_COUNT] = {
  // ---------------- ES ----------------
  {
    "Esta evolucionando!", "Nam nam!", "Le gusta!", "Tiene hambre!", "Necesita un bano!",
    "Esta agotado...", "Esta triste...", "Esta rellenito...", "Es SHINY!!", "Esta feliz",
    "GRACIAS! Hasta siempre", "Se ha escapado...", "Adios! Se despide...",
    "HUEVO", "Huevo legendario!?", "Huevo raro!", "Toca el huevo...", "Se mueve!", "Esta a punto!",
    "POKEDEX %u/%u(%u)",
    "%s%s Nv.%u",
    "Soltar a %s?", "SI", "NO",
    "%u GOLPES", "FUERZA +%u", "NUEVO RECORD!", "RECORD: %u", "APORREA RAPIDO!",
    "PUNTOS: %u", "Que felicidad!", "+felicidad",
    "AJUSTAR HORA", "HORA", "MIN", "desliza arriba: cancelar", "Idioma",
    "MEDALLA!", "GENIAL!", "RACHA %u DIAS!",
    "RACHA %u  rec %u", "VIN", "BAYA ???", "BAYA ROJA", "BAYA AZUL", "BAYA VERDE",
    "%s   EDAD %lud", "toca el nombre: renombrar",
    "COMBATE", "FUE", "DEF", "VEL", "PES", "ENTRENAR FUERZA",
    "MEDALLAS %d/%d", "toca: volver",
    "NOMBRE:", "toca para volver",
    "COM", "FEL", "ENE", "LIM",
    "REC %u",
    "PROGRESO", "Nv.%u", "%u min para Nv.%u", "EVOLUCION", "Forma final",
    "Listo para evolucionar!", "Sube todo a 40 para evolucionar",
    "Evoluciona en %u niv.", "Descuidos: %u",
    "SON ON", "SON OFF",
    "EVOLUCIONAR", "%s quiere decirte algo...", "%s se siente abandonado...",
    "Evolucionar?", "Mantener forma", "Despedirse?", "Despedirse", "Quedaros juntos",
    "Elige tu inicial",
    "Sin sprites", "Cargalos en la SD",
    "PS", "IV %u",
    "MENU", "AJUSTES", "CERRAR", "EQUIPO %u/5", "- vacio -", "%s se une al equipo!", "Equipo lleno: elige a quien sustituir", "Dejarlo ir",
    "STATS", "ENTRENAR", "FUERZA", "VELOCIDAD", "DEFENSA", "Sube sola si esta a gusto",
    "MOVIMIENTOS", "- vacio -", "Elige movimiento", "Toca para cambiar", "POT %u", "ESTADO",
    "%s quiere aprender", "No aprender",
    "%s usa %s", "Es muy eficaz!", "No es muy eficaz...", "No le afecta...",
    "%s ha fallado!", "Golpe critico!", "%s se debilita!", "Se ha herido!",
    "%s: %s", "PARALISIS", "QUEMADURA", "VENENO", "DORMIDO", "CONGELADO", "CONFUSION",
    "Has ganado!", "Has perdido...",
    "%s saca a %s", "Adelante, %s!", "GIMNASIOS", "MEDALLAS %u/8", "ENTRENADOR", "VELOCIDAD +%u", "toca: cambiar avatar", "%u en total", "NORMAL", "DIFICIL",
 "ELEGIDOS %u/%u", "LUCHAR", "BLOQUEADO", "POKEMON", "%s derrotado!", "MEDALLA NUEVA!", "VOL %u", "CAJA %u/%u", "cambiar con %s: elige hueco", "CAJA", "TRAER", "termina la accion actual", "COMBATE LAN", "CREAR", "UNIRSE", "buscando...", "listo!", "version distinta", "crear o unirse", "rival: %u mons", "el rival se fue", "esperando al rival...", "OTRA VEZ", "HUIR", "de que region viene el huevo", "%s +%u", "ya no puede entrenar mas", "ELIGE TU REGION", "RETIRAR", "Retirarla ya?", "la siguiente evoluciona un dia mas tarde", "evoluciona un dia mas tarde",   "FALTA PACK", "SOLTAR", "se va para siempre", "AL EQUIPO", "no se unira a tu equipo",
   "GUARDADO", "ENVIAR PARTIDA", "RECIBIR PARTIDA", "enviando %u%%", "recibiendo %u%%", "partida lista", "APLICAR", "reemplaza tu partida", "envio completo", "partida invalida", "CODIGO %06lu",
   "EXPLORAR", "NORMAL", "ALEATORIO", "ENERGIA %u / COSTE %u", "No hay energia suficiente", "Equipo y caja llenos", "Aparece %s salvaje!", "%s capturado!", "%s escapo...", "El actual ocupa este hueco",
   "AHORRO ON", "AHORRO OFF", "LUZ %u", "DORMIR", "DESPERTAR", "CANGURO", "CANGURO ON", "Hace falta 15+ antes de las 19", "JUGAR", "PASEO", "EMPEZAR", "SIN SENSOR", "%u pasos", "CORRER", "DESPERTAR", "FRUTA", "TOPOS", "SALTAR", "%s viene a jugar!", "SIN PUNTOS", "FELIZ +20", },
  // ---------------- EN ----------------
  {
    "Evolving!", "Yum yum!", "It likes it!", "It's hungry!", "Needs a bath!",
    "Worn out...", "Feeling sad...", "A bit chubby...", "It's SHINY!!", "It's happy",
    "THANKS! Farewell", "It ran away...", "Bye! Waving goodbye...",
    "EGG", "Legendary egg!?", "Rare egg!", "Tap the egg...", "It moves!", "Almost there!",
    "POKEDEX %u/%u(%u)",
    "%s%s Lv.%u",
    "Release %s?", "YES", "NO",
    "%u HITS", "STR +%u", "NEW RECORD!", "BEST: %u", "HIT FAST!",
    "SCORE: %u", "So much fun!", "+happiness",
    "SET TIME", "HOUR", "MIN", "swipe up: cancel", "Lang",
    "MEDAL!", "AWESOME!", "%u DAY STREAK!",
    "STREAK %u  best %u", "BOND", "BERRY ???", "RED BERRY", "BLUE BERRY", "GREEN BERRY",
    "%s   AGE %lud", "tap name: rename",
    "BATTLE", "ATK", "DEF", "SPD", "WGT", "TRAIN STRENGTH",
    "MEDALS %d/%d", "tap: back",
    "NAME:", "tap to go back",
    "FOOD", "JOY", "ENE", "HYG",
    "BEST %u",
    "PROGRESS", "Lv.%u", "%u min to Lv.%u", "EVOLUTION", "Final form",
    "Ready to evolve!", "All needs >=40 to evolve",
    "Evolves in %u lv.", "Slip-ups: %u",
    "SND ON", "SND OFF",
    "EVOLVE!", "%s wants to tell you...", "%s feels abandoned...",
    "Evolve?", "Keep form", "Say goodbye?", "Goodbye", "Stay together",
    "Choose your starter",
    "No sprites", "Load them onto the SD",
    "HP", "IV %u",
    "MENU", "SETTINGS", "CLOSE", "PARTY %u/5", "- empty -", "%s joined the party!", "Party full: pick who to replace", "Let it go",
    "STATS", "TRAINING", "STRENGTH", "SPEED", "DEFENCE", "Grows on its own when happy",
    "MOVES", "- empty -", "Choose a move", "Tap a slot to change", "PWR %u", "STATUS",
    "%s wants to learn", "Do not learn",
    "%s used %s", "Super effective!", "Not very effective...", "It had no effect...",
    "%s missed!", "Critical hit!", "%s fainted!", "It hurt itself!",
    "%s: %s", "PARALYSED", "BURNED", "POISONED", "ASLEEP", "FROZEN", "CONFUSED",
    "You win!", "You lost...",
    "%s sends out %s", "Go, %s!", "GYMS", "BADGES %u/8", "TRAINER",
 "SPEED +%u", "tap: change avatar", "%u earned in all", "EASY", "HARD", "CHOSEN %u/%u", "FIGHT", "LOCKED", "POKEMON", "%s defeated!", "NEW BADGE!", "VOL %u", "BOX %u/%u", "swap with %s: pick a slot", "BOX", "BRING BACK", "finish the current action", "LAN BATTLE", "HOST", "JOIN", "searching...", "ready!", "different version", "host or join", "rival: %u mons", "the rival left", "waiting for the rival...", "AGAIN", "RUN", "where this egg comes from", "%s +%u", "trained as far as it can go", "CHOOSE A REGION", "RETIRE", "Retire it early?", "the next one evolves a day later", "evolves a day later",
   "NEEDS PACK", "RELEASE", "gone for good", "TO PARTY", "it will not join your party",
   "SAVE TRANSFER", "SEND SAVE", "RECEIVE SAVE", "sending %u%%", "receiving %u%%", "save ready", "APPLY SAVE", "replaces your current save", "transfer complete", "invalid save", "CODE %06lu",
   "EXPLORE", "NORMAL", "RANDOM", "ENERGY %u / COST %u", "Not enough energy", "Party and box are full", "A wild %s appeared!", "%s was caught!", "%s broke free...", "Current pet takes this slot",
   "SAVE ON", "SAVE OFF", "BRT %u", "SLEEP", "WAKE", "SITTER", "SITTER ON", "Needs 15+ before 7pm", "PLAY", "WALK", "START WALK", "NO STEP SENSOR", "%u steps", "RUN", "WAKE UP", "CATCH FRUIT", "WHACK", "JUMP", "%s came to play!", "NO SCORE", "JOY +20", },
  // ---------------- FR ----------------
  {
    "Il evolue!", "Miam miam!", "Il aime ca!", "Il a faim!", "Besoin d'un bain!",
    "Epuise...", "Triste...", "Un peu rond...", "C'est SHINY!!", "Il est content",
    "MERCI! Adieu", "Il s'est enfui...", "Au revoir!",
    "OEUF", "Oeuf legendaire!?", "Oeuf rare!", "Touche l'oeuf...", "Il bouge!", "Presque la!",
    "POKEDEX %u/%u(%u)",
    "%s%s Niv.%u",
    "Relacher %s?", "OUI", "NON",
    "%u COUPS", "FORCE +%u", "NOUVEAU RECORD!", "RECORD: %u", "FRAPPE VITE!",
    "SCORE: %u", "Trop bien!", "+bonheur",
    "REGLER L'HEURE", "HEURE", "MIN", "glisse haut: annuler", "Langue",
    "MEDAILLE!", "SUPER!", "SERIE %u JOURS!",
    "SERIE %u  rec %u", "LIEN", "BAIE ???", "BAIE ROUGE", "BAIE BLEUE", "BAIE VERTE",
    "%s   AGE %lud", "touche le nom: renommer",
    "COMBAT", "ATQ", "DEF", "VIT", "PDS", "ENTRAINER FORCE",
    "MEDAILLES %d/%d", "touche: retour",
    "NOM:", "touche pour revenir",
    "NOUR", "JOIE", "ENE", "HYG",
    "REC %u",
    "PROGRES", "Niv.%u", "%u min pour Niv.%u", "EVOLUTION", "Forme finale",
    "Pret a evoluer!", "Tout a 40 pour evoluer",
    "Evolue dans %u niv.", "Negligences: %u",
    "SON ON", "SON OFF",
    "EVOLUER", "%s veut te parler...", "%s se sent abandonne...",
    "Evoluer?", "Garder forme", "Dire adieu?", "Adieu", "Rester ensemble",
    "Choisis ton starter",
    "Pas de sprites", "Charge-les sur la SD",
    "PV", "IV %u",
    "MENU", "REGLAGES", "FERMER", "EQUIPE %u/5", "- vide -", "%s rejoint l'equipe!", "Equipe pleine: qui remplacer?", "Le laisser partir",
    "STATS", "ENTRAINEMENT", "FORCE", "VITESSE", "DEFENSE", "Monte seule s'il est heureux",
    "CAPACITES", "- vide -", "Choisis une capacite", "Touche pour changer", "PUI %u", "STATUT",
    "%s veut apprendre", "Ne pas apprendre",
    "%s utilise %s", "Tres efficace!", "Peu efficace...", "Aucun effet...",
    "%s a rate!", "Coup critique!", "%s est K.O.!", "Il se blesse!",
    "%s: %s", "PARALYSIE", "BRULURE", "POISON", "ENDORMI", "GELE", "CONFUSION",
    "Gagne!", "Perdu...",
    "%s envoie %s", "Vas-y, %s!", "ARENES", "BADGES %u/8", "DRESSEUR", "VITESSE +%u", "touche: changer d'avatar", "%u au total", "NORMAL", "DIFFICILE", "CHOISIS %u/%u", "COMBATTRE", "VERROUILLE", "POKEMON", "%s vaincu!", "NOUVEAU BADGE!", "VOL %u", "BOITE %u/%u", "echanger avec %s: choisis", "BOITE", "RAMENER", "terminer l'action en cours", "COMBAT LAN", "CREER", "REJOINDRE", "recherche...", "pret!", "version differente", "creer ou rejoindre", "rival: %u mons", "le rival est parti", "en attente du rival...", "ENCORE", "FUIR", "d ou vient cet oeuf", "%s +%u", "ne peut plus progresser", "CHOISIS TA REGION", "RETIRER", "Retirer maintenant?", "le suivant evolue un jour plus tard", "evolue un jour plus tard",
   "PACK REQUIS", "RELACHER", "parti pour de bon", "A L EQUIPE", "ne rejoindra pas l equipe",
   "SAUVEGARDE", "ENVOYER", "RECEVOIR", "envoi %u%%", "reception %u%%", "partie prete", "APPLIQUER", "remplace la partie", "envoi termine", "partie invalide", "CODE %06lu",
   "EXPLORER", "NORMAL", "ALEATOIRE", "ENERGIE %u / COUT %u", "Pas assez d energie", "Equipe et boite pleines", "Un %s sauvage apparait!", "%s est capture!", "%s s echappe...", "Le compagnon prend cette place",
   "ECO ON", "ECO OFF", "LUM %u", "DORMIR", "REVEIL", "NOUNOU", "NOUNOU ON", "Il faut 15+ avant 19h", "JOUER", "MARCHE", "PARTIR", "PAS DE CAPTEUR", "%u pas", "COURIR", "REVEIL", "FRUITS", "TAUPE", "SAUT", "%s vient jouer!", "SANS SCORE", "JOIE +20", },
  // ---------------- DE ----------------
  {
    "Entwickelt sich!", "Mampf mampf!", "Gefaellt ihm!", "Hat Hunger!", "Braucht ein Bad!",
    "Erschoepft...", "Traurig...", "Etwas rundlich...", "Es ist SHINY!!", "Es ist froh",
    "DANKE! Lebwohl", "Es ist weg...", "Tschuess! Winkt",
    "EI", "Legendaeres Ei!?", "Seltenes Ei!", "Beruehre das Ei...", "Es bewegt sich!", "Fast soweit!",
    "POKEDEX %u/%u(%u)",
    "%s%s Lv.%u",
    "%s freilassen?", "JA", "NEIN",
    "%u TREFFER", "KRAFT +%u", "NEUER REKORD!", "REKORD: %u", "SCHNELL HAUEN!",
    "PUNKTE: %u", "Wie schoen!", "+Freude",
    "ZEIT STELLEN", "STD", "MIN", "hoch wischen: abbruch", "Sprache",
    "MEDAILLE!", "TOLL!", "%u TAGE SERIE!",
    "SERIE %u  rek %u", "BND", "BEERE ???", "ROTE BEERE", "BLAUE BEERE", "GRUENE BEERE",
    "%s   ALTER %lud", "Name tippen: umbenennen",
    "KAMPF", "ANG", "VER", "INI", "GEW", "KRAFT TRAINIEREN",
    "MEDAILLEN %d/%d", "tippen: zurueck",
    "NAME:", "tippen zum zurueck",
    "ESS", "FRO", "ENE", "HYG",
    "REK %u",
    "FORTSCHRITT", "Lv.%u", "%u min bis Lv.%u", "ENTWICKLUNG", "Endform",
    "Bereit zur Entwicklung!", "Alles >=40 zur Entwicklung",
    "Entwickelt in %u Lv.", "Patzer: %u",
    "TON AN", "TON AUS",
    "ENTWICKELN", "%s will dir etwas sagen...", "%s fuehlt sich verlassen...",
    "Entwickeln?", "Form behalten", "Abschied?", "Lebwohl", "Zusammen bleiben",
    "Waehle dein Starter",
    "Keine Sprites", "Auf die SD laden",
    "KP", "IV %u",
    "MENU", "EINSTELLUNGEN", "SCHLIESSEN", "TEAM %u/5", "- leer -", "%s kommt ins Team!", "Team voll: wen ersetzen?", "Ziehen lassen",
    "WERTE", "TRAINING", "STAERKE", "TEMPO", "ABWEHR", "Steigt von selbst bei guter Laune",
    "ATTACKEN", "- leer -", "Attacke waehlen", "Tippen zum Aendern", "STK %u", "STATUS",
    "%s will lernen", "Nicht lernen",
    "%s setzt %s ein", "Sehr effektiv!", "Nicht sehr effektiv...", "Keine Wirkung...",
    "%s hat verfehlt!", "Volltreffer!", "%s wurde besiegt!", "Es verletzt sich!",
    "%s: %s", "PARALYSE", "VERBRANNT", "VERGIFTET", "SCHLAEFT", "GEFROREN", "VERWIRRT",
    "Gewonnen!", "Verloren...",
    "%s schickt %s", "Los, %s!", "ARENEN", "ORDEN %u/8", "TRAINER", "TEMPO +%u", "tippen: Avatar wechseln", "%u insgesamt", "NORMAL", "SCHWER", "GEWAEHLT %u/%u", "KAEMPFEN", "GESPERRT", "POKEMON", "%s besiegt!", "NEUER ORDEN!", "LAUT %u", "BOX %u/%u", "mit %s tauschen: waehle", "BOX", "ZURUECK", "erst Aktion beenden", "LAN KAMPF", "HOSTEN", "BEITRETEN", "suche...", "bereit!", "andere Version", "hosten oder beitreten", "Gegner: %u", "der Gegner ist weg", "warte auf den Gegner...", "NOCHMAL", "FLUCHT", "woher dieses Ei kommt", "%s +%u", "kann nicht weiter trainieren", "WAEHLE DEINE REGION", "VERABSCHIEDEN", "Jetzt verabschieden?", "das naechste entwickelt sich einen Tag spaeter", "entwickelt sich einen Tag spaeter",
   "PACK FEHLT", "FREILASSEN", "fuer immer weg", "INS TEAM", "kommt nicht ins team",
   "SPIELSTAND", "SENDEN", "EMPFANGEN", "senden %u%%", "empfangen %u%%", "Spielstand bereit", "UEBERNEHMEN", "ersetzt Spielstand", "Uebertragung fertig", "ungueltiger Stand", "CODE %06lu",
   "ERKUNDEN", "NORMAL", "ZUFALL", "ENERGIE %u / KOSTEN %u", "Nicht genug Energie", "Team und Box sind voll", "Ein wildes %s erscheint!", "%s wurde gefangen!", "%s ist entkommen...", "Aktuelles Pokemon in diesen Platz",
   "SPAREN AN", "SPAREN AUS", "HELL %u", "SCHLAF", "WACH", "SITTER", "SITTER AN", "15+ vor 19 Uhr", "SPIELEN", "GEHEN", "LOS", "KEIN SENSOR", "%u Schritte", "LAUF", "WECKEN", "FRUCHT", "MAULWURF", "SPRUNG", "%s kommt spielen!", "KEINE PUNKTE", "FREUDE +20", },
  // ---------------- IT ----------------
  {
    "Si evolve!", "Gnam gnam!", "Gli piace!", "Ha fame!", "Vuole un bagno!",
    "Esausto...", "Triste...", "Un po' cicciotto...", "E' SHINY!!", "E' felice",
    "GRAZIE! Addio", "E' scappato...", "Ciao! Saluta",
    "UOVO", "Uovo leggendario!?", "Uovo raro!", "Tocca l'uovo...", "Si muove!", "Ci siamo quasi!",
    "POKEDEX %u/%u(%u)",
    "%s%s Lv.%u",
    "Liberare %s?", "SI", "NO",
    "%u COLPI", "FORZA +%u", "NUOVO RECORD!", "RECORD: %u", "COLPISCI VELOCE!",
    "PUNTI: %u", "Che gioia!", "+felicita",
    "IMPOSTA ORA", "ORA", "MIN", "scorri su: annulla", "Lingua",
    "MEDAGLIA!", "GRANDE!", "SERIE %u GIORNI!",
    "SERIE %u  rec %u", "LEG", "BACCA ???", "BACCA ROSSA", "BACCA BLU", "BACCA VERDE",
    "%s   ETA %lud", "tocca il nome: rinomina",
    "LOTTA", "ATT", "DIF", "VEL", "PES", "ALLENA FORZA",
    "MEDAGLIE %d/%d", "tocca: indietro",
    "NOME:", "tocca per tornare",
    "CIB", "GIO", "ENE", "IGI",
    "REC %u",
    "PROGRESSI", "Lv.%u", "%u min per Lv.%u", "EVOLUZIONE", "Forma finale",
    "Pronto a evolvere!", "Tutto a 40 per evolvere",
    "Evolve tra %u liv.", "Disattenzioni: %u",
    "AUD ON", "AUD OFF",
    "EVOLVI", "%s vuole dirti qualcosa...", "%s si sente abbandonato...",
    "Evolvere?", "Mantieni forma", "Salutare?", "Addio", "Restare insieme",
    "Scegli l'iniziale",
    "Senza sprite", "Caricali sulla SD",
    "PS", "IV %u",
    "MENU", "IMPOSTAZIONI", "CHIUDI", "SQUADRA %u/5", "- vuoto -", "%s entra in squadra!", "Squadra piena: chi sostituire?", "Lasciarlo andare",
    "STATS", "ALLENAMENTO", "FORZA", "VELOCITA", "DIFESA", "Sale da sola se sta bene",
    "MOSSE", "- vuoto -", "Scegli una mossa", "Tocca per cambiare", "POT %u", "STATO",
    "%s vuole imparare", "Non imparare",
    "%s usa %s", "Superefficace!", "Non molto efficace...", "Nessun effetto...",
    "%s ha mancato!", "Brutto colpo!", "%s e\' esausto!", "Si e ferito!",
    "%s: %s", "PARALISI", "SCOTTATURA", "VELENO", "ADDORMENTATO", "CONGELATO", "CONFUSIONE",
    "Hai vinto!", "Hai perso...",
    "%s manda %s", "Vai, %s!", "PALESTRE", "MEDAGLIE %u/8", "ALLENATORE", "VELOCITA +%u", "tocca: cambia avatar", "%u in totale", "NORMALE", "DIFFICILE", "SCELTI %u/%u", "LOTTA", "BLOCCATO", "POKEMON", "%s sconfitto!", "NUOVA MEDAGLIA!", "VOL %u", "BOX %u/%u", "scambia con %s: scegli", "BOX", "RIPORTA", "termina l'azione attuale", "LOTTA LAN", "CREA", "ENTRA", "ricerca...", "pronto!", "versione diversa", "crea o entra", "rivale: %u mons", "il rivale se n' e andato", "in attesa del rivale...", "ANCORA", "FUGGI", "da quale regione viene l uovo", "%s +%u", "non puo allenarsi oltre", "SCEGLI LA REGIONE", "RITIRARE", "Ritirarla adesso?", "il prossimo evolve un giorno dopo", "evolve un giorno dopo",
   "MANCA PACK", "LIBERA", "via per sempre", "AL GRUPPO", "non entrera nel gruppo",
   "SALVATAGGIO", "INVIA", "RICEVI", "invio %u%%", "ricezione %u%%", "salvataggio pronto", "APPLICA", "sostituisce il salvataggio", "invio completo", "salvataggio non valido", "CODICE %06lu",
   "ESPLORA", "NORMALE", "CASUALE", "ENERGIA %u / COSTO %u", "Energia insufficiente", "Squadra e box pieni", "Appare un %s selvatico!", "%s catturato!", "%s e fuggito...", "L'attuale prende questo posto",
   "RISPARMIO ON", "RISPARMIO OFF", "LUCE %u", "NANNA", "SVEGLIA", "TATA", "TATA ON", "Serve 15+ prima delle 19", "GIOCA", "PASSEGGIO", "INIZIA", "NIENTE SENSORE", "%u passi", "CORSA", "SVEGLIA", "FRUTTA", "TALPA", "SALTO", "%s viene a giocare!", "NIENTE PUNTI", "GIOIA +20", },
  // ---------------- PT ----------------
  {
    "Evoluindo!", "Nham nham!", "Ele gosta!", "Esta com fome!", "Precisa de banho!",
    "Exausto...", "Triste...", "Um pouco gordinho...", "E SHINY!!", "Esta feliz",
    "OBRIGADO! Adeus", "Fugiu...", "Tchau! Acena",
    "OVO", "Ovo lendario!?", "Ovo raro!", "Toque no ovo...", "Mexe-se!", "Quase la!",
    "POKEDEX %u/%u(%u)",
    "%s%s Niv.%u",
    "Soltar %s?", "SIM", "NAO",
    "%u GOLPES", "FORCA +%u", "NOVO RECORDE!", "RECORDE: %u", "BATA RAPIDO!",
    "PONTOS: %u", "Que alegria!", "+alegria",
    "AJUSTAR HORA", "HORA", "MIN", "deslize cima: cancelar", "Idioma",
    "MEDALHA!", "OTIMO!", "%u DIAS SEGUIDOS!",
    "SEQ %u  rec %u", "LACO", "BAGA ???", "BAGA VERMELHA", "BAGA AZUL", "BAGA VERDE",
    "%s   IDADE %lud", "toque no nome: renomear",
    "COMBATE", "ATQ", "DEF", "VEL", "PES", "TREINAR FORCA",
    "MEDALHAS %d/%d", "toque: voltar",
    "NOME:", "toque para voltar",
    "COM", "ALE", "ENE", "HIG",
    "REC %u",
    "PROGRESSO", "Niv.%u", "%u min para Niv.%u", "EVOLUCAO", "Forma final",
    "Pronto a evoluir!", "Tudo a 40 para evoluir",
    "Evolui em %u niv.", "Descuidos: %u",
    "SOM ON", "SOM OFF",
    "EVOLUIR", "%s quer dizer-te algo...", "%s sente-se abandonado...",
    "Evoluir?", "Manter forma", "Despedir?", "Adeus", "Ficar juntos",
    "Escolhe o inicial",
    "Sem sprites", "Carrega-os no SD",
    "PS", "IV %u",
    "MENU", "AJUSTES", "FECHAR", "EQUIPA %u/5", "- vazio -", "%s junta-se a equipa!", "Equipa cheia: quem substituir?", "Deixa-lo ir",
    "STATS", "TREINO", "FORCA", "VELOCIDADE", "DEFESA", "Sobe sozinha se estiver bem",
    "GOLPES", "- vazio -", "Escolhe um golpe", "Toca para mudar", "POT %u", "ESTADO",
    "%s quer aprender", "Nao aprender",
    "%s usa %s", "Super eficaz!", "Pouco eficaz...", "Nao teve efeito...",
    "%s falhou!", "Acerto critico!", "%s desmaiou!", "Feriu-se!",
    "%s: %s", "PARALISIA", "QUEIMADURA", "VENENO", "A DORMIR", "CONGELADO", "CONFUSAO",
    "Ganhaste!", "Perdeste...",
    "%s envia %s", "Vai, %s!", "GINASIOS", "MEDALHAS %u/8", "TREINADOR", "VELOCIDADE +%u", "toca: mudar avatar", "%u no total", "NORMAL", "DIFICIL", "ESCOLHIDOS %u/%u", "LUTAR", "BLOQUEADO", "POKEMON", "%s derrotado!", "NOVA MEDALHA!", "VOL %u", "CAIXA %u/%u", "trocar com %s: escolhe", "CAIXA", "TRAZER", "termina a acao atual", "COMBATE LAN", "CRIAR", "ENTRAR", "a procurar...", "pronto!", "versao diferente", "criar ou entrar", "rival: %u mons", "o rival saiu", "a esperar pelo rival...", "OUTRA VEZ", "FUGIR", "de que regiao vem o ovo", "%s +%u", "ja nao pode treinar mais", "ESCOLHE A REGIAO", "REFORMAR", "Reformar agora?", "o proximo evolui um dia depois", "evolui um dia depois",
   "FALTA PACK", "SOLTAR", "vai para sempre", "A EQUIPA", "nao entrara na equipa",
   "GRAVACAO", "ENVIAR", "RECEBER", "a enviar %u%%", "a receber %u%%", "gravacao pronta", "APLICAR", "substitui a gravacao", "envio concluido", "gravacao invalida", "CODIGO %06lu",
   "EXPLORAR", "NORMAL", "ALEATORIO", "ENERGIA %u / CUSTO %u", "Energia insuficiente", "Equipa e caixa cheias", "Um %s selvagem apareceu!", "%s foi capturado!", "%s escapou...", "O atual fica neste lugar",
   "POUPAR ON", "POUPAR OFF", "LUZ %u", "DORMIR", "ACORDAR", "BABA", "BABA ON", "Precisa 15+ antes das 19", "BRINCAR", "CAMINHADA", "COMECAR", "SEM SENSOR", "%u passos", "CORRER", "ACORDAR", "FRUTA", "TOUPEIRA", "PULAR", "%s veio brincar!", "SEM PONTOS", "ALEGRIA +20", },
  // ---------------- ZH ----------------
  {
    "進化中！", // S_EVOLVING
    "好好味！", // S_EATING
    "好喜歡！", // S_LIKES
    "肚餓了！", // S_HUNGRY
    "想洗澡！", // S_NEEDS_BATH
    "好累…", // S_EXHAUSTED
    "好傷心…", // S_SAD
    "有點胖胖的", // S_CHUBBY
    "異色寶可夢！", // S_IS_SHINY
    "好開心", // S_HAPPY
    "多謝你！保重！", // S_FAREWELL
    "離開了…", // S_RUNAWAY
    "再見！走好！", // S_GOODBYE
    "蛋", // S_EGG_HDR
    "傳說之蛋！？", // S_EGG_LEGEND
    "稀有的蛋！", // S_EGG_RARE
    "摸摸蛋吧", // S_EGG_TOUCH
    "在動了！", // S_EGG_MOVES
    "快要出生了！", // S_EGG_ALMOST
    "圖鑑 %u/%u(%u)", // S_POKEDEX_FMT
    "%s%s Lv.%u", // S_NAME_FMT
    "要放生%s嗎？", // S_RELEASE_FMT
    "是", // S_YES
    "否", // S_NO
    "%u 次擊中", // S_HITS_FMT
    "攻擊 +%u", // S_STR_GAIN_FMT
    "新紀錄！", // S_NEW_RECORD
    "最高紀錄：%u", // S_RECORD_FMT
    "快點拍！", // S_HIT_FAST
    "分數：%u", // S_SCORE_FMT
    "超級開心！", // S_GREAT_JOY
    "心情上升", // S_PLUS_JOY
    "設定時間", // S_SET_TIME
    "時", // S_HOUR
    "分", // S_MIN
    "向上滑動取消", // S_CLOCK_CANCEL
    "語言", // S_LANG_LABEL
    "獲得獎章！", // S_MEDAL_BANNER
    "太棒了！", // S_GREAT
    "連續 %u 日！", // S_STREAK_DAYS_FMT
    "連續 %u 日 / 最高 %u 日", // S_STREAK_FMT
    "羈絆", // S_VIN
    "樹果 ???", // S_BERRY_UNK
    "紅色樹果", // S_BERRY_RED
    "藍色樹果", // S_BERRY_BLUE
    "綠色樹果", // S_BERRY_GREEN
    "%s / 年齡 %lu 日", // S_INFO_FMT
    "點按名字改名", // S_RENAME_HINT
    "能力", // S_BATTLE
    "攻擊", // S_STAT_ATK
    "防禦", // S_STAT_DEF
    "速度", // S_STAT_SPE
    "體重", // S_STAT_WGT
    "攻擊訓練", // S_TRAIN_STR
    "獎章 %d/%d", // S_MEDALS_FMT
    "返回", // S_BACK
    "名字：", // S_NAME
    "點按返回", // S_DETAIL_BACK
    "飽足", // S_BAR_FOOD
    "心情", // S_BAR_JOY
    "活力", // S_BAR_ENE
    "清潔", // S_BAR_HYG
    "最高 %u", // S_REC_FMT
    "成長", // S_PROGRESS
    "Lv.%u", // S_LVL_FMT
    "%u 分鐘後 Lv.%u", // S_NEXT_LVL_FMT
    "進化", // S_EVO_LABEL
    "最終進化形態", // S_FINAL_FORM
    "可以進化了！", // S_EVO_READY
    "所有狀態需達 40 以上", // S_EVO_BLOCKED
    "%u 級後進化", // S_EVO_IN_FMT
    "照顧失誤：%u", // S_MISTAKES_FMT
    "音效開", // S_SND_ON
    "音效關", // S_SND_OFF
    "進化", // S_EVO_TAP
    "%s 有話想說", // S_FAREWELL_BTN
    "%s 感到寂寞", // S_RUNAWAY_BTN
    "要進化嗎？", // S_EVO_Q
    "保持現狀", // S_EVO_KEEP
    "要告別嗎？", // S_FAR_Q
    "告別", // S_FAR_GO
    "繼續相伴", // S_FAR_STAY
    "選擇第一位夥伴", // S_CHOOSE_STARTER
    "沒有圖片", // S_NO_SPRITES
    "請在 SD 卡安裝圖片", // S_LOAD_SPRITES
    "體力", // S_STAT_VIT
    "個體值 %u", // S_IV_FMT
    "選單", // S_MENU_TITLE
    "設定", // S_SETTINGS
    "關閉", // S_CLOSE
    "隊伍 %u/5", // S_PARTY_FMT
    "空位", // S_PARTY_EMPTY
    "%s 加入隊伍！", // S_PARTY_JOINED
    "隊伍已滿，請先交換", // S_PARTY_FULL
    "放生", // S_PARTY_LETGO
    "查看狀態", // S_STATS
    "訓練", // S_TRAIN
    "攻擊", // S_TR_ATK
    "速度", // S_TR_SPE
    "防禦", // S_TR_DEF
    "選擇要訓練的能力", // S_TR_DEF_HINT
    "招式", // S_MOVES
    "空位", // S_MOVE_EMPTY
    "選擇招式", // S_MOVE_PICK
    "點按招式更換", // S_MOVE_TAP
    "威力 %u", // S_MOVE_PWR
    "變化", // S_MOVE_STATUS
    "%s 要學的招式是？", // S_LEARN_Q
    "不學習", // S_LEARN_SKIP
    "%s 使出 %s！", // S_BTL_USED
    "效果絕佳！", // S_BTL_SUPER
    "效果好像不太好…", // S_BTL_WEAK
    "沒有效果！", // S_BTL_IMMUNE
    "%s 沒有命中！", // S_BTL_MISS
    "擊中要害！", // S_BTL_CRIT
    "%s 倒下了！", // S_BTL_FAINT
    "混亂中攻擊了自己！", // S_BTL_HURTSELF
    "%s：%s", // S_BTL_STATUS
    "麻痺", // S_AIL_PARA
    "灼傷", // S_AIL_BURN
    "中毒", // S_AIL_POISON
    "睡眠", // S_AIL_SLEEP
    "冰凍", // S_AIL_FREEZE
    "混亂", // S_AIL_CONFUSE
    "勝利！", // S_BTL_WIN
    "落敗…", // S_BTL_LOSE
    "%s：%s 登場！", // S_BTL_SENDS
    "去吧，%s！", // S_BTL_GO
    "道館", // S_GYMS
    "徽章 %u/8", // S_BADGES_FMT
    "訓練家", // S_TRAINER
    "速度 +%u", // S_SPD_GAIN_FMT
    "點按更換外觀", // S_AVATAR_HINT
    "累計 %u 個", // S_MEDALS_TOTAL_FMT
    "普通", // S_EASY
    "困難", // S_HARD
    "選擇 %u/%u", // S_PICK_FMT
    "對戰", // S_FIGHT
    "未解鎖", // S_LOCKED
    "交換", // S_BTL_SWITCH
    "擊敗了 %s！", // S_BTL_BEAT
    "獲得新徽章！", // S_BTL_NEWBADGE
    "音量 %u", // S_VOL_FMT
    "箱子 %u/%u", // S_BOX_FMT
    "%s，選擇要交換的位置", // S_BOX_SWAP
    "箱子", // S_BOX_BTN
    "帶回來", // S_REVIVE
    "請先完成進行中的選擇", // S_REVIVE_EGG
    "近距離對戰", // S_LAN
    "建立房間", // S_LAN_HOST
    "加入房間", // S_LAN_JOIN
    "正在尋找對手…", // S_LAN_WAIT
    "準備完成！", // S_LAN_READY
    "版本不同", // S_LAN_REFUSED
    "請建立或加入房間", // S_LAN_PICK
    "對手：%u 隻", // S_LAN_VS
    "對手離開了", // S_LAN_GONE
    "正在等待對手…", // S_LAN_WAITFOE
    "再戰一次", // S_LAN_REMATCH
    "逃跑", // S_BTL_RUN
    "蛋會在哪個地區出生", // S_EGG_REGION
    "%s +%u", // S_WIN_TRAIN_FMT
    "已達訓練上限", // S_WIN_MAXED
    "選擇地區", // S_CHOOSE_REGION
    "結束照顧", // S_RETIRE
    "現在結束照顧嗎？", // S_RETIRE_Q
    "下一隻寶可夢會延遲一日進化", // S_RETIRE_COST
    "進化會延遲一日", // S_EVO_SLOW
    "需要圖片包", // S_NEED_PACK
    "放生", // S_RELEASE_BTN
    "將永遠離開", // S_RELEASE_GONE
    "放入隊伍", // S_BOX_TAKE
    "不會留在隊伍", // S_RETIRE_GONE
    "傳送存檔", // S_SAVE_TITLE
    "傳送存檔", // S_SAVE_SEND
    "接收存檔", // S_SAVE_RECEIVE
    "傳送中 %u%%", // S_SAVE_SENDING_FMT
    "接收中 %u%%", // S_SAVE_RECEIVING_FMT
    "接收完成", // S_SAVE_READY
    "套用存檔", // S_SAVE_APPLY
    "會覆蓋目前存檔", // S_SAVE_OVERWRITE
    "傳送完成", // S_SAVE_DONE
    "存檔無效", // S_SAVE_INVALID
    "確認碼 %06lu", // S_SAVE_CODE_FMT
    "探索", // S_EXPLORE
    "普通", // S_EXPLORE_NORMAL
    "隨機", // S_EXPLORE_RANDOM
    "活力 %u / 消耗 %u", // S_EXPLORE_COST_FMT
    "活力不足", // S_EXPLORE_NEED_ENERGY
    "隊伍和箱子都已滿", // S_EXPLORE_STORAGE_FULL
    "野生的 %s 出現了！", // S_WILD_MET_FMT
    "成功捕獲 %s！", // S_WILD_CAUGHT_FMT
    "捕獲 %s 失敗…", // S_WILD_ESCAPED_FMT
    "目前的寶可夢會移到此格", // S_ACTIVE_SWAP_HINT
    "省電：開", // S_PSAVE_ON
    "省電：關", // S_PSAVE_OFF
    "亮度 %u", // S_BRIGHT_FMT
    "睡覺", // S_SLEEP
    "起床", // S_WAKE
    "保姆", // S_SITTER
    "保姆照顧中", // S_SITTER_ON
    "七時前且高於 15", // S_SITTER_NO
    "一起玩", // S_PLAY
    "散步", // S_WALK
    "開始散步", // S_WALK_GO
    "讀不到步數", // S_WALK_NO
    "%u 步", // S_STEPS_FMT
    "跑步皮卡丘", // S_GAME_RUN
    "叫醒卡比獸", // S_GAME_WAKE
    "接果實", // S_GAME_CATCH
    "打地鼠", // S_GAME_DIG
    "跳鯉魚王", // S_GAME_JUMP
    "%s 來玩了！", // S_FRIEND_VISIT
    "沒有得分", // S_NO_SCORE
    "開心 +20", // S_PLAY_DONE
  },
};

// Nombres de medalla en sus tres longitudes [idioma][medalla].
static const char *const MED_NAME[LANG_COUNT][MED_COUNT] = {
  { "Nv.10", "Nv.25", "Nv.50", "BAYA", "RACHA 7", "VINCULO", "FORMA TOPE", "EN FORMA" },
  { "Lv.10", "Lv.25", "Lv.50", "BERRY", "7 STREAK", "BOND", "TOP FORM", "IN SHAPE" },
  { "Niv.10", "Niv.25", "Niv.50", "BAIE", "SERIE 7", "LIEN", "FORME MAX", "EN FORME" },
  { "Lv.10", "Lv.25", "Lv.50", "BEERE", "7 SERIE", "BINDUNG", "ENDFORM", "FIT" },
  { "Lv.10", "Lv.25", "Lv.50", "BACCA", "SERIE 7", "LEGAME", "FORMA MAX", "IN FORMA" },
  { "Niv.10", "Niv.25", "Niv.50", "BAGA", "SEQ 7", "LACO", "FORMA MAX", "EM FORMA" },
  { "Lv.10", "Lv.25", "Lv.50", "樹果", "連續7日", "羈絆", "最終進化", "健康" },
};
static const char *const MED_LBL[LANG_COUNT][MED_COUNT] = {
  { "Nv10", "Nv25", "Nv50", "BAYA", "7DIAS", "VINC", "TOPE", "SANO" },
  { "Lv10", "Lv25", "Lv50", "BERRY", "7DAYS", "BOND", "TOP", "FIT" },
  { "Niv10", "Niv25", "Niv50", "BAIE", "7JRS", "LIEN", "MAX", "FORME" },
  { "Lv10", "Lv25", "Lv50", "BEERE", "7TAGE", "BND", "END", "FIT" },
  { "Lv10", "Lv25", "Lv50", "BACCA", "7GG", "LEG", "MAX", "FIT" },
  { "Niv10", "Niv25", "Niv50", "BAGA", "7DIAS", "LACO", "MAX", "FIT" },
  { "Lv10", "Lv25", "Lv50", "樹果", "7日", "羈絆", "進化", "健康" },
};
static const char *const MED_DSC[LANG_COUNT][MED_COUNT] = {
  { "NIVEL 10", "NIVEL 25", "NIVEL 50", "BAYA HALLADA",
    "RACHA 7 DIAS", "VINCULO MAX", "FORMA FINAL", "EN FORMA" },
  { "LEVEL 10", "LEVEL 25", "LEVEL 50", "BERRY FOUND",
    "7 DAY STREAK", "MAX BOND", "FINAL FORM", "IN SHAPE" },
  { "NIVEAU 10", "NIVEAU 25", "NIVEAU 50", "BAIE TROUVEE",
    "SERIE 7 JOURS", "LIEN MAX", "FORME FINALE", "EN FORME" },
  { "LEVEL 10", "LEVEL 25", "LEVEL 50", "BEERE GEFUNDEN",
    "7 TAGE SERIE", "MAX BINDUNG", "ENDFORM", "FIT" },
  { "LIVELLO 10", "LIVELLO 25", "LIVELLO 50", "BACCA TROVATA",
    "SERIE 7 GIORNI", "LEGAME MAX", "FORMA FINALE", "IN FORMA" },
  { "NIVEL 10", "NIVEL 25", "NIVEL 50", "BAGA ACHADA",
    "SEQ 7 DIAS", "LACO MAX", "FORMA FINAL", "EM FORMA" },
  { "等級 10", "等級 25", "等級 50", "喜歡的樹果", "連續照顧 7 日", "最高羈絆", "最終進化形態", "健康的身體" },
};

const char *T(StrId id) { return STRINGS[gLang][id]; }
const char *medalName(int i)  { return MED_NAME[gLang][i]; }
const char *medalLabel(int i) { return MED_LBL[gLang][i]; }
const char *medalDesc(int i)  { return MED_DSC[gLang][i]; }

void loadLang() {
  Preferences p;
  p.begin("tamapoke", true);  // solo lectura
  uint8_t v = p.getUChar("lang", LANG_DEFAULT);
  p.end();
  gLang = (v < LANG_COUNT) ? (Lang)v : LANG_DEFAULT;
}

void setLang(Lang l) {
  if (l >= LANG_COUNT) return;
  gLang = l;
  Preferences p;
  p.begin("tamapoke", false);
  p.putUChar("lang", (uint8_t)l);
  p.end();
}

#include "zh_names.h"
#include <string.h>
const char *localName(const char *en) {
  return gLang == LANG_ZH ? zhName(en) : en;
}

const char *zhName(const char *en) {
  if (!en) return en;
  int lo=0, hi=sizeof(ZH_NAMES)/sizeof(ZH_NAMES[0])-1;
  while (lo<=hi) { int mid=(lo+hi)/2; int c=strcmp(en,ZH_NAMES[mid].en);
    if (!c) return ZH_NAMES[mid].zh; if (c<0) hi=mid-1; else lo=mid+1; }
  return en;
}
