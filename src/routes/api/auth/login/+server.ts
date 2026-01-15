import { json, redirect } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { authenticateUser, createSession } from '$lib/server/auth';

export const POST: RequestHandler = async ({ request, cookies }) => {
  const formData = await request.formData();
  const email = formData.get('email') as string;
  const password = formData.get('password') as string;

  if (!email || !password) {
    return new Response(null, {
      status: 303,
      headers: {
        Location: '/login?error=missing_fields',
      },
    });
  }

  const user = await authenticateUser(email, password);

  if (!user) {
    return new Response(null, {
      status: 303,
      headers: {
        Location: '/login?error=invalid_credentials',
      },
    });
  }

  const sessionToken = await createSession(user.id);

  cookies.set('session', sessionToken, {
    path: '/',
    httpOnly: true,
    sameSite: 'lax',
    secure: process.env.NODE_ENV === 'production',
    maxAge: 60 * 60 * 24 * 7, // 7 days
  });

  return new Response(null, {
    status: 303,
    headers: {
      Location: '/dashboard',
    },
  });
};
