const test = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');

const firmwarePath = path.join(__dirname, '..', '..', 'esp32-firmware', 'solarbeam_firmware.ino');
const firmware = fs.readFileSync(firmwarePath, 'utf8');

test('o firmware nao desliga a bomba automaticamente em modo manual', () => {
  assert.doesNotMatch(
    firmware,
    /if \(modoOperacao != "automatico" && bombaLigada\(\)\) definirBomba\(false\);/,
    'O firmware não deve forçar desligamento automático quando o modo é manual.'
  );
});
