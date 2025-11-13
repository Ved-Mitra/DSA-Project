from manim import *
import pandas as pd
import networkx as nx
import numpy as np

class SNIEAnimation(Scene):

    def construct(self):
        nodes_df = pd.read_csv("nodes.csv")
        edges_df = pd.read_csv("edges.csv")
        rec_df   = pd.read_csv("recommedation.csv")

        nodes_df["id"] = nodes_df["id"].astype(str)
        edges_df["u"]  = edges_df["u"].astype(str)
        edges_df["v"]  = edges_df["v"].astype(str)
        rec_df["ID"]   = rec_df["ID"].astype(str)

        recommended = rec_df["ID"].tolist()
        start_node  = recommended[0]

        G = nx.Graph()

        for _, row in nodes_df.iterrows():
            G.add_node(
                row["id"],
                domain=int(row["domain"]),
                strength=float(row["strength"])
            )

        for _, row in edges_df.iterrows():
            G.add_edge(row["u"], row["v"], weight=float(row["weight"]))

        pos2d = nx.spring_layout(G, seed=42, k=3.5, iterations=300)

        pos2d = {n: xy * 3.0 for n, xy in pos2d.items()}

        pos = {n: np.array([xy[0], xy[1], 0]) for n, xy in pos2d.items()}

        domain_colors = {1: RED, 2: GREEN, 3: BLUE, 4: ORANGE}

        node_config = {
            n: {
                "fill_color": domain_colors[G.nodes[n]["domain"]],
                "radius": 0.18 ,
                "stroke_width": 2
            }
            for n in G.nodes()
        }

        edge_config = {
            e: {"stroke_width": 2, "stroke_color": GRAY}
            for e in G.edges()
        }

        mg = Graph(
            list(G.nodes()),
            list(G.edges()),
            layout=pos,
            layout_scale=4.0,
            vertex_config=node_config,
            edge_config=edge_config
        )

        # =====================================================
        # PHASE 1 — ORIGINAL NETWORK
        # =====================================================
        title1 = Text("SNIE — Phase 1: Original Network", font_size=36).to_edge(UP)
        self.play(Write(title1))
        self.play(Create(mg), run_time=3)
        self.wait(1)

        title2 = Text("SNIE — Phase 2: Suggested Bridges", font_size=36).to_edge(UP)
        self.play(Transform(title1, title2), run_time=1)

        dashed_edges = []

        for target in recommended:
            if target in G.nodes:
                line = DashedLine(
                    mg[start_node].get_center(),
                    mg[target].get_center(),
                    stroke_color=YELLOW,
                    stroke_width=4,
                    dash_length=0.2
                )
                dashed_edges.append(line)

        for d in dashed_edges:
            self.play(Create(d), run_time=0.7)

        self.wait(1)

        title3 = Text("SNIE — Phase 3: Final Network After Bridges", font_size=36).to_edge(UP)
        self.play(Transform(title1, title3), run_time=1)

        solid_edges = []

        for target in recommended:
            if target in G.nodes:
                line = Line(
                    mg[start_node].get_center(),
                    mg[target].get_center(),
                    stroke_color=YELLOW,
                    stroke_width=5
                )
                solid_edges.append(line)

        for s in solid_edges:
            self.play(Create(s), run_time=0.7)

        self.play(*[FadeOut(d) for d in dashed_edges])

        self.wait(2)
