import { PrismaClient } from '@prisma/client';
import * as fs from 'fs';
import * as path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const prisma = new PrismaClient();

const LESSONS_DIR = path.join(__dirname, 'lessons');

// Mapping slug exercice -> fichier lesson
const lessonMapping: Record<string, string> = {
  // Module 1: Anti-Debug Basics
  'antidebug-01-ptrace': '01-ptrace.md',
  'antidebug-02-sysctl': '02-sysctl.md',
  'antidebug-03-getppid': '03-getppid.md',
  'antidebug-04-isatty': '04-isatty.md',
  'antidebug-05-multi': '05-multi.md',
  'antidebug-06-sigtrap': '06-sigtrap.md',
};

async function main() {
  console.log('Adding lessons to exercises...\n');

  for (const [slug, lessonFile] of Object.entries(lessonMapping)) {
    const lessonPath = path.join(LESSONS_DIR, lessonFile);

    if (!fs.existsSync(lessonPath)) {
      console.log(`  [SKIP] ${slug} - lesson file not found: ${lessonFile}`);
      continue;
    }

    const theory = fs.readFileSync(lessonPath, 'utf-8');

    try {
      await prisma.exercise.update({
        where: { slug },
        data: { theory },
      });
      console.log(`  [OK] ${slug}`);
    } catch (error) {
      console.log(`  [ERROR] ${slug} - exercise not found`);
    }
  }

  console.log('\nDone!');
}

main()
  .catch(console.error)
  .finally(() => prisma.$disconnect());
