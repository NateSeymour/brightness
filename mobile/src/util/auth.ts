export interface LoginUrlOptions {
  authority: string,
  clientId: string,
  responseType: string,
  scope: string[],
  redirectUri: string,
}

export const buildLoginUrl = (opts: LoginUrlOptions) => {
  const url = new URL('/login', opts.authority);

  url.searchParams.append('client_id', opts.clientId);
  url.searchParams.append('response_type', opts.responseType);
  url.searchParams.append('scope', opts.scope.join('+'));
  url.searchParams.append('redirect_uri', opts.redirectUri);

  return url.toString().replace('%2B', '+');
};