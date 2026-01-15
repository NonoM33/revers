FROM node:20-slim AS builder

WORKDIR /app

# Install dependencies
COPY package*.json ./
RUN npm ci

# Copy prisma schema and generate client
COPY prisma ./prisma
RUN npx prisma generate

# Copy source and build
COPY . .
RUN npm run build

# Initialize database with seed data during build
RUN mkdir -p /app/data
ENV DATABASE_URL="file:/app/data/reverse_academy.db"
RUN npx prisma db push
RUN npx tsx prisma/seed.ts
RUN npx tsx prisma/add-lessons.ts

# Production image
FROM node:20-slim AS runner

WORKDIR /app

# Install OpenSSL for Prisma
RUN apt-get update && apt-get install -y openssl && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN groupadd -r -g 1001 nodejs && useradd -r -u 1001 -g nodejs sveltekit

# Copy built application and pre-seeded database
COPY --from=builder /app/build ./build
COPY --from=builder /app/node_modules ./node_modules
COPY --from=builder /app/package.json ./
COPY --from=builder /app/prisma ./prisma
COPY --from=builder /app/static ./static
COPY --from=builder /app/data ./data

# Set environment
ENV NODE_ENV=production
ENV PORT=3000
ENV DATABASE_URL="file:/app/data/reverse_academy.db"

# Ensure data directory is writable
RUN chown -R sveltekit:nodejs /app/data

USER sveltekit

EXPOSE 3000

CMD ["node", "build"]
