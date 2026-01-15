FROM node:20-alpine AS builder

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

# Production image
FROM node:20-alpine AS runner

WORKDIR /app

# Create non-root user
RUN addgroup --system --gid 1001 nodejs
RUN adduser --system --uid 1001 sveltekit

# Copy built application
COPY --from=builder /app/build ./build
COPY --from=builder /app/node_modules ./node_modules
COPY --from=builder /app/package.json ./
COPY --from=builder /app/prisma ./prisma
COPY --from=builder /app/static ./static

# Set environment
ENV NODE_ENV=production
ENV PORT=3000
ENV DATABASE_URL="file:/app/data/reverse_academy.db"

# Create data directory for SQLite
RUN mkdir -p /app/data && chown -R sveltekit:nodejs /app/data

USER sveltekit

EXPOSE 3000

# Initialize database and start
CMD npx prisma db push && node build
