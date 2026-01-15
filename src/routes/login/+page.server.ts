import type { PageServerLoad, Actions } from './$types';
import { redirect, fail } from '@sveltejs/kit';
import { authenticateUser, createSession } from '$lib/server/auth';

export const load: PageServerLoad = async ({ locals }) => {
  // Redirect if already logged in
  if (locals.user) {
    throw redirect(303, '/dashboard');
  }

  return {};
};

export const actions: Actions = {
  default: async ({ request, cookies }) => {
    try {
      const formData = await request.formData();
      const email = formData.get('email') as string;
      const password = formData.get('password') as string;

      if (!email || !password) {
        return fail(400, { error: 'Veuillez remplir tous les champs.' });
      }

      const user = await authenticateUser(email, password);

      if (!user) {
        return fail(401, { error: 'Email ou mot de passe incorrect.' });
      }

      const sessionToken = await createSession(user.id);

      cookies.set('session', sessionToken, {
        path: '/',
        httpOnly: true,
        sameSite: 'lax',
        secure: false, // Disable for HTTP
        maxAge: 60 * 60 * 24 * 7, // 7 days
      });

      throw redirect(303, '/dashboard');
    } catch (e) {
      // Re-throw redirects
      if (e && typeof e === 'object' && 'status' in e && (e as any).status === 303) {
        throw e;
      }
      console.error('Login error:', e);
      return fail(500, { error: 'Erreur serveur. Veuillez reessayer.' });
    }
  },
};
