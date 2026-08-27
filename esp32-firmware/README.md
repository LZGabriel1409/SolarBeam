# Firmware SolarBeam

O botão de gravação do navegador usa `esptool-js` e espera um único arquivo
`.bin` mesclado, contendo bootloader, tabela de partições e aplicação.

Gere esse arquivo depois de compilar o projeto pela Arduino IDE ou PlatformIO.
Com o `esptool.py` instalado, um exemplo de mesclagem para ESP32 é:

```bash
esptool.py --chip esp32 merge_bin \
  -o solarbeamfirmware.bin \
  --flash_mode dio --flash_freq 40m --flash_size 4MB \
  0x1000 build/solarbeam.ino.bootloader.bin \
  0x8000 build/solarbeam.ino.partitions.bin \
  0xe000 build/boot_app0.bin \
  0x10000 build/solarbeam.ino.bin
```

Selecione `solarbeamfirmware.bin` em `Dispositivos`, conecte o ESP32
por USB e clique em `Gravar Firmware`. Depois da gravação, o navegador envia o
código do dispositivo e aguarda a primeira leitura chegar à API.

O flash pelo navegador exige Chrome ou Edge, HTTPS (ou `localhost`) e um ESP32
conectado por USB. O dispositivo deve estar em modo de bootloader quando a
gravação começar; normalmente isso acontece automaticamente, mas algumas
placas exigem segurar `BOOT` durante a conexão.

Nas releases do projeto, baixe `solarbeam-firmware-release.zip` para receber o
firmware mesclado, o checksum e estas instruções em um único pacote.

## Configuração do Wi-Fi pelo portal cativo

Depois de gravar o firmware, o ESP32 abre a rede Wi-Fi `SolarBeam` quando não
possui uma rede salva. Conecte o celular ou computador nessa rede; o portal de
configuração deve abrir automaticamente. Se não abrir, acesse:

```text
http://192.168.4.1
```

Informe o SSID e a senha da rede local. Se o ESP32 ficar mais de 60 segundos
sem conseguir conectar ao Wi-Fi salvo, ele abandona a tentativa e abre o mesmo
portal novamente para permitir a troca de rede.