import type { RequestHandler } from './$types';
import { deleteSession } from '$lib/server/auth';

export const POST: RequestHandler = async ({ cookies }) => {
  const sessionToken = cookies.get('session');

  if (sessionToken) {
    deleteSession(sessionToken);
  }

  cookies.delete('session', { path: '/' });

  return new Response(null, {
    status: 303,
    headers: {
      Location: '/',
    },
  });
};
