# Diagrams
> Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
> Author: Shchapaniak Andrei <xshcha00@vutbr.cz>
> Date: 13.5.2022

## Class diagram editor
Unsupported operations:
- It is not possible to copy/paste a line, as well as objects with liтes
Supported operations
- Anything else. You are free to click every button in the application, and see how it works.
- Adding methods, attributes. Deleting them. Every command is atomic, so it is possible to undo/redo it without causing inconsistencies


## Sequence diagram editor
Unsupported operatios


-----------------------trida
Aplikace načte ze souboru textové parametry diagramu třídy a zobrazí jej se všemi potřebnými komponentami, tj. název třídy, atributy s datovými typy vč. podpory čtyř možných modifikátorů přístupu ( + , - , # , ~ ) a metody. Obdobně pro rozhraní.
Třídy/Rozhraní mohou být dále propojeny pomocí vztahů: asociace, agregace, kompozice a generalizace.
V případě generalizace budou přepsané metody v odvozených třídách vizuálně odlišeny.



-----------------------------sekvenční
Aplikace načte textové parametry sekvenčního diagramu a zobrazí jej se všemi potřebnými komponentami (časová osa, objekty).
Objekty mohou spolu dále interagovat pomocí zpráv, které mohou být těchto typů: synchronní zpráva, asynchronní zpráva, návrat zprávy, tvorba objektu, zánik (uvolnění) objektu.
Implementujte podporu aktivace, vytváření a rušení objektů.