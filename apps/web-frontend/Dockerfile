# Use a specific Node.js image compatible with ARM
FROM node:14-alpine

# Expose ports for the application
EXPOSE 8080
EXPOSE 8888

# Set the working directory
WORKDIR /app

# Set proper ownership for the app directory and switch to the node user
RUN chown -R node:node /app
USER node

# Verify the user environment
RUN id node

# Copy dependency files
COPY package.json yarn.lock ./

# Install dependencies using Yarn
RUN yarn install --frozen-lockfile

# Copy application source files
COPY . .

# Default command to start the application
CMD ["yarn", "run", "dev"]