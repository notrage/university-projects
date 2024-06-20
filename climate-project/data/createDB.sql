create table Departements (
    code_departement TEXT,
    nom_departement TEXT,
    code_region INTEGER,
    zone_climatique TEXT,
    constraint pk_departements primary key (code_departement),
    constraint fk_region foreign key (code_region) references Regions(code_region)
);

create table Regions (
    code_region INTEGER,
    nom_region TEXT,
    constraint pk_regions primary key (code_region)
);

create table Mesures (
    code_departement TEXT,
    date_mesure DATE,
    temperature_min_mesure FLOAT,
    temperature_max_mesure FLOAT,
    temperature_moy_mesure FLOAT,
    constraint pk_mesures primary key (code_departement, date_mesure),
    constraint fk_mesures foreign key (code_departement) references Departements(code_departement)
);

--TODO Q4 Ajouter les créations des nouvelles tables

CREATE TABLE Communes (
    code_commune INT,
    code_departement TEXT,
    nom_commune TEXT,
    status_commune TEXT,
    altitude_moyenne_commune INT,
    population_commune INT,
    superficie_commune INT,
    code_canton_commune INT,
    code_arrondissement_commune INT,
    constraint pk_communes primary key (code_commune, code_departement),
    constraint fk_communes foreign key (code_departement) references Departements(code_departement)
);

CREATE TABLE Travaux (
    id_travaux INT PRIMARY KEY,
    cout_total_ht_travaux FLOAT,
    cout_induis_ht_travaux FLOAT,
    annee_travaux INT,
    type_logement_travaux TEXT,
    annee_construction_travaux TEXT,
    code_region INT,
    CONSTRAINT fk_travaux FOREIGN KEY (code_region) REFERENCES Regions(code_region)
);

CREATE TABLE TravauxChauffage (
    id_travaux INT PRIMARY KEY,
    energie_avant_travaux_chauffage TEXT,
    energie_installee_chauffage TEXT,
    generateur_chauffage TEXT,
    type_chaudiere_chauffage TEXT,
    CONSTRAINT fk_travaux_chauffage FOREIGN KEY (id_travaux) REFERENCES Travaux(id_travaux)
    --CONSTRAINT ck_travaux_chauffage_energie_avant_travaux_enum CHECK (energie_avant_travaux_chauffage IN ('GAZ', 'FIOUL', 'ELECTRICITE', 'BOID', 'AUTRES', null)),
    --CONSTRAINT ck_travaux_chauffage_energie_installee_enum CHECK (energie_installee_chauffage IN ('GAZ', 'FIOUL', 'ELECTRICITE', 'BOID', 'AUTRES', null)),
    --CONSTRAINT ck_travaux_chauffage_generateur_enum CHECK (generateur_chauffage IN ('CHAUDIERE', 'PAC', 'AUTRES', 'INSERT', 'POELE', 'RADIATEUR', null)),
    --CONSTRAINT ck_travaux_chauffage_type_chaudiere_enum CHECK (type_chaudiere_chauffage IN ('STANDARD', 'AIR-EAU', 'A CONDENSATION', 'AUTRES', 'AIR-AIR', 'GEOTHERMIE', 'HPE', null))
);

CREATE TABLE TravauxIsolation (
    id_travaux INT PRIMARY KEY,
    poste_isolation TEXT,
    isolant_isolation TEXT,
    epaisseur_isolation INT,
    surface_isolation INT,
    CONSTRAINT fk_travaux_isolation FOREIGN KEY (id_travaux) REFERENCES Travaux(id_travaux)
    --CONSTRAINT ck_travaux_isolation_poste CHECK (poste_isolation IN ('COMBLES PERDUES', '|T|', '|TE', 'RAMPANTS', 'SARKING', 'TOITURE TERRASSE', 'PLANCHER BAS', null)),
    --CONSTRAINT ck_travaux_isolation_isolant CHECK (isolant_isolation IN ('LAINE VEGETALE', 'LAINE MINERALE', 'PLASTIQUES', 'AUTRES', null))
);

CREATE TABLE TravauxPhotovoltaique (
    id_travaux INT PRIMARY KEY,
    puissance_installee_photovoltaique INT,
    type_panneau_photovoltaique TEXT,
    CONSTRAINT fk_travaux_photovoltaique FOREIGN KEY (id_travaux) REFERENCES Travaux(id_travaux)
    --CONSTRAINT ck_travaux_photovoltaique_type_panneau CHECK (type_panneau_photovoltaique IN ('MONOCRISTALLIN', 'POLYCRISTALLIN', null))
);