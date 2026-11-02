# APD – Laboratoare (LabWindows/CVI)

Proiectele se compilează în **LabWindows/CVI pe Windows**. Liniile adăugate la rezolvarea temei sunt marcate cu `//extra:`.

## lab03 – Simulare achiziție semnale
Fișiere: `AcquisitionSimulation.c/.h/.uir`, `Signals.c/.h`. Enunț: `docs/ApdL03-SignalGeneration.pdf`.

1. În CVI: File → New → Project, apoi adaugi toate cele 5 fișiere în proiect.
2. Build → Run. Rezolvate: To do 01–10.

## lab04 – Comunicație serială RS232 + Pan-Tompkins
Proiect: `SerialECG.prj` (se deschide direct în CVI). Enunț: `docs/ApdLAB04_new.pdf`.

1. Creezi o pereche de porturi virtuale COM1 ↔ COM2 (VSPD sau com0com).
2. Pornești `tools/SimulatorECG/ApdL04-SimulatorEcg.exe` (cu fișierul `rx` lângă): Power On → port COM1 → Deschide fișier.
3. Rulezi `SerialECG`: Setări → Setări port serial (COM2, 9600, 8, 1, fără paritate) → Deschide com port.

Rezolvat: deschidere/închidere port, callback de recepție, trimitere text (`ComWrt`), parsarea pachetelor `<v1 v2 v3>` (și când vin rupte în mai multe citiri), 3 stripchart-uri, Pan-Tompkins pe canalul D3 (To do 1–3), afișarea BPM.
