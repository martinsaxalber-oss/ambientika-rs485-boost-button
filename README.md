# Ambientika RS485 Boost Button

Manueller Boost für den **Ambientika ADVANCED B+** über einen herkömmlichen
230-V-Wandtaster – umgesetzt mit Arduino UNO und MAX485-RS485-Transceiver.
Kein WiFi, keine App, keine Fernbedienung nötig.

> ⚠️ **Unverbindliches Anwendungsbeispiel.** Die verwendeten Komponenten
> (Arduino, MAX485, Koppelrelais, Netzteil) sind kein offizielles
> Ambientika-Zubehör und werden von SUEDWIND nicht geliefert oder supportet.
> Arbeiten an der 230-V-Installation dürfen ausschließlich von einer
> qualifizierten Elektrofachkraft unter Beachtung der geltenden Vorschriften
> (z. B. CEI 64-8 / VDE 0100) ausgeführt werden. Vor allen Arbeiten ist die
> Anlage allpolig spannungsfrei zu schalten. Keine Gewähr für Funktion,
> Sicherheit oder Normkonformität; keine Haftung für Schäden aus der Umsetzung.

## Funktion

- **1. Tastendruck:** Gerät wechselt für 20 Minuten in Abluft Stufe 3 (Boost, max. 60 m³/h).
- **2. Tastendruck während des Boosts:** Boost wird vorzeitig beendet.
- Nach Ablauf bzw. Abbruch kehrt das Gerät automatisch in den Automatikmodus zurück.
- Vor jedem Stufen-/Richtungswechsel wird der Motor 10 s pausiert (Antriebsschutz).
- Der integrierte Feuchtesensor bleibt unabhängig davon aktiv.

## Stückliste

| Einbauort | Komponente | Funktion |
|---|---|---|
| Verteiler | Koppelrelais 230 V (z. B. Finder 39.11.8.230.0060) | 230-V-Tastimpuls -> potenzialfreier Kontakt |
| Verteiler | Hutschienen-Netzteil 5 V DC (z. B. Mean Well HDR-15-5) | Versorgung Arduino + MAX485 |
| Logikgehäuse | Arduino UNO R3 (oder kompatibel) | Zustandsautomat, Tasterauswertung, Protokoll |
| Logikgehäuse | MAX485-Modul | UART -> RS485 |
| Gerät (Busende) | Abschlusswiderstand 120 Ω | Busterminierung A<->B (Klemme 4 <-> 3) |

## Verkabelung RS485

| Signal | Geräteklemme ADVANCED B+ | Hinweis |
|---|---|---|
| A (+) | Klemme 4 | verdrillt mit B |
| B (-) | Klemme 3 | verdrillt mit A |
| GND | Klemme 5 | gemeinsame Masse zur Logikeinheit |

Geschirmtes, verdrilltes Kabel verwenden (z. B. J-Y(St)Y 2×2×0,8). Datenleitungen
nicht gemeinsam mit 230-V-Leitern führen. Schutzleiter (grün-gelb) niemals als
Daten- oder GND-Leiter verwenden. Busförmige Topologie, keine Sterntopologie.
120-Ω-Abschluss am letzten Busteilnehmer.

## Anschlüsse Logikeinheit

| Anschlusspunkt | Verbunden mit |
|---|---|
| Netzteil 5 V (+) | Arduino 5 V, MAX485 VCC |
| Netzteil GND (-) | Arduino GND, MAX485 GND, Relais COM, Klemme 5 (GND) |
| Relais Schließer (NO) | Arduino Pin 2 (INPUT_PULLUP) |
| MAX485 RO | Arduino Pin 10 (RX) |
| MAX485 DI | Arduino Pin 11 (TX) |
| MAX485 DE & RE | Arduino Pin 3 |
| MAX485 A / B | Klemme 4 (A) / Klemme 3 (B) |

## Inbetriebnahme

1. DIP-Schalter am ADVANCED B+ auf **„Master mit Wandpanel"** stellen.
2. Serielle Parameter: **9600 Baud, 8N1**.
3. 120-Ω-Abschluss zwischen Klemme 3 und 4 kontrollieren.
4. Sketch laden, Anlage einschalten.
5. Keine Reaktion? A und B (Klemme 3/4) tauschen – häufigste Ursache.
6. Tasterfunktion testen.

## Lizenz

MIT – siehe [LICENSE](LICENSE).
