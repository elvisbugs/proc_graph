import os
projectName = input("Digite o nome do projeto que quer renomear: ")
newName = input("Digite o novo nome que o projeto terá: ")

listNames = []

#Cria a lista dos arquivos para renomear
for i in range(4):
    if i == 0:
        listNames.append(projectName + ".sln")
        listNames.append(newName + ".sln")
    elif i == 1:
        listNames.append(projectName + ".vcxproj")
        listNames.append(newName + ".vcxproj")
    elif i == 2:
        listNames.append(listNames[2] + ".filters")
        listNames.append(listNames[3] + ".filters")
    else:
        listNames.append(listNames[2] + ".user")
        listNames.append(listNames[3] + ".user")

#renomea os arquivos
for i in range(0,8,2):
    os.rename(listNames[i], listNames[i+1])
    f = open(listNames[i+1],"r+")
    contents = f.read()
    f.seek(0)
    contents = contents.replace(projectName, newName)
    f.write(contents)
    f.truncate()
    f.close()

#adiciona no .gitignore as pastas para serem ignoradas
f = open("..\\" + ".gitignore", "a")

contents  = "\n\n/" + newName + "/x64"
contents += "\n/" + newName + "/Debug"
contents += "\n/" + newName + "/.vs"

f.write(contents)
f.close()
