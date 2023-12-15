from flask import Flask, request, jsonify
import psycopg2

app = Flask(__name__)

# Configuração do banco de dados PostgreSQL
db_params = {
    'dbname': 'BANCO DE DADOS',
    'user': 'NOMEDOUSUARIO',
    'password': 'SENHA',
    'host': 'URL DATABASE',
    'port': '5432'

}

@app.route('/enviar-dados', methods=['POST'])
def enviar_dados():
    try:
        data = request.get_json()
        conn = psycopg2.connect(**db_params)
        cursor = conn.cursor()
        cursor.execute("UPDATE leituras SET valor = %s , tempo =%s, sensor  = %s;", (data['valor1'], data['valor2'],data['valor3']))
        conn.commit()
        cursor.close()
        conn.close()
        return jsonify({'mensagem': 'Dados atualizados com sucesso!'})
    except Exception as e:
        return jsonify({'erro': str(e)})


@app.route('/obter-dados', methods=['GET'])
def obter_dados():
    try:
        conn = psycopg2.connect(**db_params)
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM leituras;")
        data = cursor.fetchall()
        cursor.close()
        conn.close()
        return jsonify(data)
    except Exception as e:
        return jsonify({'erro': str(e)})

if __name__ == '__main__':
    app.run(host='HOST SERVER PYTHON', port=5000)
    #app.run(host='', port=5000)
