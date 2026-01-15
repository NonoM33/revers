import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { prisma } from '$lib/server/db';
import fs from 'fs';

export const GET: RequestHandler = async () => {
  const debug: Record<string, unknown> = {
    timestamp: new Date().toISOString(),
    nodeVersion: process.version,
    platform: process.platform,
    arch: process.arch,
  };

  // Check database file
  const dbPath = process.env.DATABASE_URL?.replace('file:', '') || '';
  debug.databaseUrl = process.env.DATABASE_URL;
  debug.dbFileExists = fs.existsSync(dbPath);

  if (debug.dbFileExists) {
    try {
      const stats = fs.statSync(dbPath);
      debug.dbFileSize = stats.size;
      debug.dbFilePermissions = stats.mode.toString(8);
    } catch (e) {
      debug.dbStatError = e instanceof Error ? e.message : String(e);
    }
  }

  // Test Prisma connection
  try {
    const userCount = await prisma.user.count();
    debug.prismaStatus = 'OK';
    debug.userCount = userCount;
  } catch (e) {
    debug.prismaStatus = 'ERROR';
    debug.prismaError = e instanceof Error ? e.message : String(e);
    debug.prismaErrorStack = e instanceof Error ? e.stack : undefined;
  }

  // Test module query
  try {
    const moduleCount = await prisma.module.count();
    debug.moduleCount = moduleCount;
  } catch (e) {
    debug.moduleError = e instanceof Error ? e.message : String(e);
  }

  return json(debug, { status: 200 });
};
