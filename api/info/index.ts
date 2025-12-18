export const handler = async (event) => {
  return {
    statusCode: 200,
    body: JSON.stringify({
      version: 'v0.0.1',
    }),
  };
};
