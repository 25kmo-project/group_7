FROM node:20-alpine

# Asennetaan tzdata, jotta TZ-muuttuja toimii Alpinessa
RUN dpk add --no-cache tzdata

# Asetetaan aikavyöhyke
ENV TZ=Europe/Helsinki

# Aseta työskentelyhakemisto kontin sisälle
WORKDIR /usr/src/app

# Kopioi ensin vain package.json ja package-lock.json
COPY package*.json ./

# Asenna projektin riippuvuudet
RUN npm install

# Kopioi loput sovelluksen koodista
COPY . .

# Paljasta portti, jolla sovellus toimii
EXPOSE 3000

# Oletuskomento tuotantoon
CMD ["npm", "start"]