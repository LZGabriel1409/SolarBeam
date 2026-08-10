require('dotenv').config();

const obrigatorias = ['JWT_SECRET', 'TURSO_DATABASE_URL', 'TURSO_AUTH_TOKEN'];
const faltando = obrigatorias.filter((variavel) => !process.env[variavel]);

if (faltando.length > 0) {
  console.error(`Variaveis de ambiente faltando: ${faltando.join(', ')}`);
  process.exit(1);
}

const app = require('./app');
const { initDatabase } = require('./database/database');

const PORT = process.env.PORT || 3000;

async function start() {
  try {
    await initDatabase();
    app.listen(PORT, () => {
      console.log(`Servidor rodando na porta ${PORT}`);
    });
  } catch (err) {
    console.error('Falha ao inicializar o banco de dados:', err);
    process.exit(1);
  }
}

start();
