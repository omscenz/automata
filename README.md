# Documentacion

# Analizador Lexico

Este programa analiza instrucciones de asignacion simples usando expresiones regulares en C++.

Ejemplo de instruccion valida:

```
x = 25;
```

El programa verifica:

- Identificador
- Operador de asignacion
- Valor
- Terminador `;`

Tambien detecta errores y muestra mensajes descriptivos.

---

# Expresiones Regulares Utilizadas

## Identificador

```
^[a-zA-Z_][a-zA-Z0-9_]*$
```

Descripcion:

- Debe comenzar con letra o `_`
- Puede contener letras, numeros y `_`

Ejemplos validos:

```
x
_variable
valor1
```

Ejemplos invalidos:

```
1x
mi-variable
```

---

## Numero Entero

```
^-?\d+$
```

Descripcion:

- Permite numeros enteros
- Puede incluir signo negativo

Ejemplos:

```
25
-10
0
```

---

## Numero Decimal

```
^-?\d+\.\d+$
```

Descripcion:

- Permite numeros con punto decimal
- Puede incluir signo negativo

Ejemplos:

```
3.14
-2.5
```

---

## Cadena de Texto

```
^"[^"]*"$
```

Descripcion:

- Debe comenzar y terminar con `"`
- Permite texto interno

Ejemplos:

```
"Hola"
"Texto"
```

---

## Cadena Sin Cerrar

```
^"[^"]*$
```

Descripcion:

- Detecta cadenas que empiezan con `"` pero no terminan correctamente

Ejemplo:

```
"Hola
```

---

## Booleanos

```
^(true|false)$
```

Descripcion:

- Solo acepta `true` o `false`

Ejemplos:

```
true
false
```

---

# Flujo del Programa

## 1. Lectura de instruccion

El usuario escribe una instruccion.

Ejemplo:

```
edad = 20;
```

---

## 2. Tokenizacion

La instruccion se divide en:

- Identificador
- Operador
- Valor

Ejemplo:

```
edad | = | 20;
```

---

## 3. Validacion del identificador

Se verifica:

- Que no este vacio
- Que no empiece con numero
- Que no tenga caracteres invalidos

---

## 4. Validacion del valor

El programa revisa si el valor es:

- Cadena
- Entero
- Decimal
- Booleano
- Identificador

---

## 5. Validacion del terminador

Se verifica que exista `;`

---

## 6. Generacion del reporte

El programa muestra:

- Si la instruccion es valida
- O los errores encontrados

---

# Instrucciones de Ejecucion

## Compilar el programa

```cmd
g++ main.cpp -o main
```

---

## Ejecutar modo usuario

```cmd
.\main.exe
```

---

## Ejecutar instruccion directa

```powershell
.\main --instruccion "x = 25;"
```

---

# Ejemplos de Salida

## Ejemplo valido

Entrada:

```
nombre = "Juan";
```

Salida:

```
[VALIDO]

- Identificador : nombre
- Operador      : =
- Valor         : "Juan" (cadena)
- Terminador    : ;
```

---

## Ejemplo invalido

Entrada:

1x = 20;
```

Salida:

[INVALIDO]

Error 1:
Identificador invalido "1x":
Los identificadores no pueden comenzar con numeros
```

---

## Ejemplo con cadena sin cerrar

Entrada:


texto = "Hola;
```

Salida:


[INVALIDO]

Error 1:
Cadena de texto sin cerrar


---

# Autor

Proyecto realizado en C++ usando:

- Expresiones regulares
- Validacion lexica
- Analisis de tokens

