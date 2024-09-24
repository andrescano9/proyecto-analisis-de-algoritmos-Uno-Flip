# proyecto-analisis-de-algoritmos-Uno-Flip



Pontificia Universidad Javeriana
Departamento de Ingeniería de Sistemas
Análisis de algoritmos
Proyecto 2024-30

1 Descripción del problema
EL UNO (https://www.unorules.com/) es un juego de cartas muy popular que se puede jugar entre 2 y 10 jugadores.
Además, del juego básico existen variantes que lo hacen más divertido para jugar; una de ellas es la variante Flip (https:
//www.unorules.com/uno-flip-rules/).

2 Descripción del proyecto
El objetivo del presente proyecto es diseñar y construir un algoritmo que juegue una partida de UNO-Flip. Además, debe
construir una interfaz sencilla que permita un juego de entre 2 y 10 jugadores (humanos o sintéticos). NOTA: No es necesario
que el juego sea en línea con varios computadores conectados a un servidor.


3 Entregas
Según las fechas denidas en el programa del curso, este proyecto tendrá dos entregas:

1. Interfaz del juego: será el documento que contenga el diseño de la interfaz descrita más arriba.
  
2. Jugador sintético: un algoritmo que se pueda conectar a la interfaz anterior para jugar el juego propuesto.

Entrar  a la carpeta

cd ruta\al\directorio\del\proyecto


COMPILAR POWERSHELL

g++ -o uno_flip main.cpp Mazo.cpp Carta.cpp

.\uno_flip.exe




Puntos a Considerar
Reabastecimiento Automático:

La lógica para reabastecer el mazo desde el mazo de descarte ya está implementada en la función sacarCarta de la clase Mazo. Esto permite que el juego continúe sin problemas cuando el mazo se queda vacío.
Manejo de Excepciones:

No es necesario lanzar excepciones si nunca deberían estar vacíos ambos mazos. En su lugar, asegúrate de que el mazo de descarte se transfiera correctamente al mazo principal.
Efectos de las Cartas:

Actualmente, el método jugarTurno actualiza la carta activa al jugar una carta. Considera implementar la lógica para manejar efectos especiales de las cartas (por ejemplo, "salto", "reversa", etc.).
Interfaz de Usuario:

Asegúrate de que la interacción con el usuario sea clara. Puedes mejorar los mensajes que se muestran a los jugadores y gestionar mejor las entradas inválidas.
Fin del Juego:

Al finalizar el juego, se imprime un mensaje para el jugador que gana. Considera si quieres agregar opciones adicionales, como reiniciar el juego o mostrar un resumen de las partidas.
