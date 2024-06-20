import tkinter as tk
from utils import display
from tkinter import ttk

class Window(tk.Toplevel):
    def __init__(self, parent):
        super().__init__(parent)

        # Définition de la taille de la fenêtre, du titre et des lignes/colonnes de l'affichage grid
        display.centerWindow(600, 400, self)
        self.title('Q1 : départements de la région Auvergne-Rhône-Alpes')
        display.defineGridDisplay(self, 1, 1)
        #ttk.Label(self, text="Modifier cette fonction en s'inspirant du code de F1, pour qu'elle affiche la liste des départements (code_departement, nom_departement) de la région Auvergne-Rhône-Alpes",
        #         wraplength=500, anchor="center", font=('Helvetica', '10', 'bold')).grid(sticky="we", row=0)

        #TODO Q1 Modifier la suite du code (en se basant sur le code de F1) pour répondre à Q1
        columns = ('code_departement', 'nom_departement')
        query = """SELECT code_departement, nom_departement
                    FROM Regions JOIN Departements USING (code_region) 
                    WHERE nom_region = 'AUVERGNE RHONE ALPES'
                    ORDER BY code_departement"""
        # On définit les colonnes que l'on souhaite afficher dans la fenêtre et la requête

        # On utilise la fonction createTreeViewDisplayQuery pour afficher les résultats de la requête
        tree = display.createTreeViewDisplayQuery(self, columns, query,300)
        tree.grid(row=0, sticky="nswe")