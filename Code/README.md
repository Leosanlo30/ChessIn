# I. Módulo de Entrada y Parsing (PGN Miner) 📚

| Concepto | Propósito | Implementación C++ | Lógica Clave |
|---|---|---|---|
| Lectura de PGN | Ingestar partidas y procesarlas línea por línea. | `std::ifstream`, `std::getline()`, bucles `while` (para acumular líneas). | **Saltar Encabezados:** ignorar líneas que comienzan con `[` |
| Tokenización | Dividir la cadena de movimientos en unidades individuales. | `std::stringstream` (para separar por espacios). | **Concatenación con Espacio:** asegurar el espaciado correcto entre líneas con `+= " "` |
| Filtrado de Jugadas | Aislar solo los movimientos válidos de ajedrez. | `std::isalpha(token[0])` | **Regla:** los movimientos válidos deben comenzar con una letra (e.g., `e4`, `Nf3`), permitiendo ignorar `1.`, `2.`, y `1-0`. |

---

# II. Módulo de Gestión del Tablero y FEN ♟️

| Concepto | Propósito | Herramienta C++ | Lógica Clave |
|---|---|---|---|
| Board State | Mantener el estado actual del tablero después de cada jugada. | Librería de ajedrez dedicada (para bitboards). | **Generación Secuencial:** por cada token de movimiento, se llama a `board.make_move(token)` para generar el nuevo FEN. |
| Comando UCI | Preparar la instrucción para el motor. | Construcción de cadena: `position startpos moves [lista de jugadas]` | **Conexión:** traduce la lista de C++ (`std::vector<string>`) al formato de entrada que Stockfish espera. |

---

# III. Módulo de Comunicación con el Motor (Stockfish) 🔌

| Concepto | Propósito | Función Clave C++ | Lógica Clave |
|---|---|---|---|
| Comunicación IPC | Abrir y mantener un canal de comunicación con el ejecutable Stockfish. | Pipes (`popen` / `_popen` / APIs nativas) y `execStockfish()` | **Protocolo UCI:** enviar comandos (`go depth...`) y leer la respuesta por la tubería de salida. |
| Parsing de Salida | Extraer el valor numérico de la evaluación. | `parseEvaluation()` y `std::string::find("score cp")` | **Búsqueda por Palabra Clave:** evitar errores de posición variable del texto. **Conversión:** usar `std::stoi()` para convertir el texto (`"150"`) a número (`150`). |

---

# IV. Módulo de Filtro Táctico (Core Logic) 🎯

| Concepto | Propósito | Implementación C++ | Lógica Clave |
|---|---|---|---|
| Delta de Evaluación | Medir la magnitud del error (blunder). | `std::abs(evalActual - evalPrevia)` | **Valor Absoluto:** asegura que el swing se mide en magnitud, sin importar si la ventaja es para Blancas (positivo) o Negras (negativo). |
| Umbral de Táctica | Definir cuándo un swing es decisivo. | Constante: `> 200 cp` (centipeones). | **Umbral:** `200 cp` representa una ventaja de dos peones, considerada decisiva para generar un puzzle. |

---

# V. Módulo Avanzado (Escala y Dificultad) 📈

| Concepto | Propósito | Implementación C++ / Sistema | Lógica Clave |
|---|---|---|---|
| Escalabilidad | Acelerar el proceso para millones de partidas. | `std::thread` (multihilo / concurrencia). | **Paralelismo:** ejecutar múltiples instancias de `execStockfish()` al mismo tiempo, aprovechando los núcleos de la CPU. |
| Dificultad | Asignar un rating preciso al puzzle. | Glicko-2 | **Desviación (RD):** un **RD** alto en un puzzle nuevo permite ajustes de rating más grandes y rápidos debido a la falta de confianza en la evaluación inicial. |
