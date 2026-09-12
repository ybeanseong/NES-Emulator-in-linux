# NES-Emulator-in-linux
<img width="1774" height="887" alt="Image" src="https://github.com/user-attachments/assets/b7f05b52-3a16-4fa2-a1da-6ca983e3be08" />
C로 NES (Nintendo Entertainment System)을 직접 구현하면서
CPU, Memory Bus, PPU (Pixel Processing Unit) 및 APU (Audio Processing Unit)을 개발하면서
게임기를 에뮬레이팅하여 Linux 환경에서 동작하도록 학습하는 프로젝트입니다.

## Goal
- MOS 6502 CPU Emulator 구현
- NES Memory Bus 구현
- iNES ROM Loader 구현
- PPU 구현
- SDL을 통한 Linux GUI 출력
- Audio 출력
- Super Mario Bros 실행

## Current Progress
- [V] Virtual Cartridge 구현
- [V] Memory Bus 구현
- [V] CPU 구현
- Instruction 구현
   - [V] LDA :
   - [V] STA :
   - [V] INX :
   - [ ] BEQ / BNE :
   - [ ] CMP :
   - [ ] ADC / SBC :
   - [ ] Stack :
   - [ ] Interrupt :
   - [ ] ROM Loader
   - [ ] PPU
   - [ ] SDL Display :

## Architecture

CPU
 ↓
Bus
 ├── RAM
 ├── PPU
 └── Cartridge

 ## Build
 gcc ...


   
