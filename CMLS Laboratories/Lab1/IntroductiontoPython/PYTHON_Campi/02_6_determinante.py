import numpy as np
def creaSotto(m, er,ec):      # Crea una sottomatrice di m (nxn) 
   mm=[]                      # Copia tutto tranne la riga er e la colonna ec
   n=len(m)
 
   for r in range(n):
       if(r != er): 
           riga=[]
           for c in range(n):
               if(c != ec):
                   riga.append(m[r][c])
           mm.append(riga)   
   return mm
 
def determinante(m):          # Calcola il determinante
   nr=len(m)                    
   nc=len(m[0])
   if(nr != nc):              # Se la matrice non è quadrata...
       return 0        
   if(nr == 1 ):              # Se è uno scalare...
       return m[0][0]
 
   somma=0
   segno=+1
   for r in range(nr):        # Sviluppa la 1° colonna...
       mm     = creaSotto(m, r,0) 
       somma += segno*m[r][0]*determinante(mm)
       segno *= -1
   return somma


m1=[[1,1,1],[1,1,1],[1,1,1]]
print(m1)
print(determinante(m1))
print(np.linalg.det(m1))

m1=np.array([[1,2,3],[4,5,6],[7,8,9]])
print(m1)
print(determinante(m1))
print(np.linalg.det(m1))#è uno zero male arrotondato

m1=np.random.random((5,5))
print(m1)
print(determinante(m1))
print(np.linalg.det(m1))

m1=np.random.random((8,8))
print(m1)
print(determinante(m1))
print(np.linalg.det(m1))