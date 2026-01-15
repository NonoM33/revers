import type { PageServerLoad } from './$types';
import { redirect } from '@sveltejs/kit';

export const load: PageServerLoad = async ({ locals, url }) => {
  // Redirect if already logged in
  if (locals.user) {
    throw redirect(303, '/dashboard');
  }

  const error = url.searchParams.get('error');

  let errorMessage = null;
  if (error === 'missing_fields') {
    errorMessage = 'Veuillez remplir tous les champs.';
  } else if (error === 'invalid_credentials') {
    errorMessage = 'Email ou mot de passe incorrect.';
  }

  return {
    error: errorMessage,
  };
};
