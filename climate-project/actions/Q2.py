import tkinter as tk
from utils import display
from tkinter import ttk

class Window(tk.Toplevel):
    def __init__(self, parent):
        super().__init__(parent)

        # Définition de la taille de la fenêtre, du titre et des lignes/colonnes de l'affichage grid
        display.centerWindow(600, 400, self)
        self.title('Q2 : département le plus froid par région')
        display.defineGridDisplay(self, 2, 1)

        #TODO Q2 Modifier la suite du code (en se basant sur le code de F1) pour répondre à Q2

        # On définit les colonnes que l'on souhaite afficher dans la fenêtre et la requête
        columns = ('nom_departement', 'nom_region','temp_moy')
        query = """
                WITH Temp_moy_per_departement AS (
                    SELECT nom_region, nom_departement, AVG(temperature_moy_mesure) AS temp_moy
                    FROM Departements JOIN Mesures USING (code_departement)
                                      JOIN Regions USING (code_region)  
                    GROUP BY nom_region, nom_departement)
                SELECT nom_departement, nom_region, ROUND(temp_moy,2)
                FROM Temp_moy_per_departement
                WHERE (nom_region, temp_moy) IN (
                    SELECT nom_region, MIN(temp_moy)
                    FROM Temp_moy_per_departement
                    GROUP BY nom_region
                )
                """
        # On utilise la fonction createTreeViewDisplayQuery pour afficher les résultats de la requête
        tree = display.createTreeViewDisplayQuery(self, columns, query,200)
        tree.grid(row=0, sticky="nswe")